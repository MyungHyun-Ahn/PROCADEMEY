#include "pch.h"
#include "Session.h"
#include "CLanServer.h"

CLanServer *g_Server;

unsigned AcceptorThreadFunc(LPVOID lpParam)
{
	return g_Server->AccepterThread();
}

unsigned WorkerThreadFunc(LPVOID lpParam)
{
	return g_Server->WorkerThread();
}

bool CLanServer::Start(const char *openIP, const USHORT port, USHORT createWorkerThreadCount, USHORT maxWorkerThreadCount, INT maxClientCount)
{
	InitializeSession(maxClientCount);

	WSAData wsaData;
	int retVal;
	int errVal;

	retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != retVal)
	{
		wprintf(L"[ERROR] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == m_listenSocket)
	{
		errVal = WSAGetLastError();
		wprintf(L"[ERROR] socket()함수 실패 : %d\n", errVal);
		return false;
	}

	m_maxWorkerThreadCount = maxWorkerThreadCount;

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, openIP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);

	retVal = bind(m_listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (SOCKET_ERROR == retVal)
	{
		errVal = WSAGetLastError();
		wprintf(L"[ERROR] bind()함수 실패 : %d\n", errVal);
		return false;
	}


	retVal = listen(m_listenSocket, SOMAXCONN);
	if (0 != retVal)
	{
		errVal = WSAGetLastError();
		wprintf(L"[ERROR] listen()함수 실패 : %d\n", errVal);
		return false;
	}

	LINGER ling;
	ling.l_linger = 0;
	ling.l_onoff = 1;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_LINGER, (char *)&ling, sizeof(ling));

	// SendBuffer 0으로
	DWORD sendBufferSize = 0;
	setsockopt(m_listenSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sendBufferSize, sizeof(sendBufferSize));

	// CP 핸들 생성
	m_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, m_maxWorkerThreadCount);
	if (NULL == m_IOCPHandle)
	{
		errVal = GetLastError();
		wprintf(L"[ERROR] CreateIoCompletionPort()함수 실패: %d\n", errVal);
		return false;
	}

	// CreateAcceptThread
	m_AccepterThread = (HANDLE)_beginthreadex(nullptr, 0, AcceptorThreadFunc, nullptr, CREATE_SUSPENDED, nullptr);
	if (m_AccepterThread == 0)
	{
		wprintf(L"[ERROR] AccepterThread running fail..\n");
		return false;

	}
	wprintf(L"[SYSTEM] AccepterThread running..\n");

	// CreateWorkerThread
	for (int i = 1; i <= createWorkerThreadCount; i++)
	{
		HANDLE hWorkerThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThreadFunc, nullptr, CREATE_SUSPENDED, nullptr);
		if (hWorkerThread == 0)
		{
			wprintf(L"[ERROR] WorkerThread[%d] running fail..\n", i);
			return false;
		}

		m_WorkerThreads.push_back(hWorkerThread);
		wprintf(L"[SYSTEM] WorkerThread[%d] running..\n", i);
	}

	ResumeThread(m_AccepterThread);
	for (int i = 0; i < createWorkerThreadCount; i++)
	{
		ResumeThread(m_WorkerThreads[i]);
	}

	return true;
}

void CLanServer::SendPacket(const UINT64 sessionId, SerializableBuffer *sBuffer)
{
	UINT64 index = GetSessionIndex(sessionId);

	Session *pSession = m_pArrSession[index];

	if (pSession->m_SessionID != sessionId)
		__debugbreak();

	USHORT header = sBuffer->GetDataSize();
	sBuffer->EnqueueHeader((char *)&header, sizeof(USHORT));
	pSession->SendPacket(sBuffer);
}

bool CLanServer::Disconnect(Session *pSession)
{
	InterlockedExchange(&pSession->m_IsValid, FALSE);
	return true;
}

bool CLanServer::ReleaseSession(Session *pSession)
{
	USHORT index = GetSessionIndex(pSession->m_SessionID);
	closesocket(pSession->m_ClientSocket);
	pSession->Clear();

	Session::Free(pSession);
	// delete pSession;

	AcquireSRWLockExclusive(&m_DisconnectStackLock);
	m_DisconnectIndexStack.push_back(index);
	ReleaseSRWLockExclusive(&m_DisconnectStackLock);

	return true;
}

void CLanServer::Monitor()
{
	m_AcceptTPS = 0;
	m_RecvMsgTPS = 0;
	m_SendMsgTPS = 0;
}

int CLanServer::WorkerThread()
{
	int retVal;
	DWORD flag = 0;
	while (m_isWorkerRun)
	{
		DWORD dwTransferred = 0;
		Session *pSession = 0;
		OverlappedEx *lpOverlapped = 0;

		retVal = GetQueuedCompletionStatus(m_IOCPHandle, &dwTransferred, (PULONG_PTR)&pSession, (LPOVERLAPPED *)&lpOverlapped, INFINITE);

		if (lpOverlapped == nullptr)
		{
			if (retVal == FALSE)
			{
				// GetQueuedCompletionStatus Fail
				PostQueuedCompletionStatus(m_IOCPHandle, 0, 0, 0);
				break;
			}

			if (dwTransferred == 0 && pSession == 0)
			{
				// 정상 종료 루틴
				PostQueuedCompletionStatus(m_IOCPHandle, 0, 0, 0);
				break;
			}
		}
		// 소켓 정상 종료
		else if (dwTransferred == 0)
		{
			Disconnect(pSession);
		}
		else
		{
			if (lpOverlapped->m_Operation == IOOperation::RECV)
			{
				pSession->RecvCompleted(dwTransferred);

				if (!pSession->PostSend())
					ReleaseSession(pSession);

				if (!pSession->PostRecv())
					ReleaseSession(pSession);
			}
			else if (lpOverlapped->m_Operation == IOOperation::SEND)
			{
				pSession->SendCompleted(dwTransferred);

				if (!pSession->PostSend())
					ReleaseSession(pSession);
			}
		}

		LONG back = InterlockedDecrement(&pSession->m_ioCount);
		if (back == 0)
			ReleaseSession(pSession);
	}

	return 0;
}

int CLanServer::AccepterThread()
{
	int errVal;

	while (1)
	{
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		
		DWORD flag = 0;

		// 10038

		if (m_listenSocket == INVALID_SOCKET)
			__debugbreak();

		SOCKET clientSocket = accept(m_listenSocket, (SOCKADDR *)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			errVal = WSAGetLastError();

			wprintf(L"[ERROR] accept() Error : %d\n", errVal);
			return false;
		}

		WCHAR clientAddrBuf[16] = { 0, };
		InetNtop(AF_INET, &clientAddr.sin_addr, clientAddrBuf, 16);

		// 접속 허용 IP 확인
		if (!OnConnectionRequest(clientAddrBuf, ntohs(clientAddr.sin_port)))
			continue;

		AcquireSRWLockExclusive(&m_DisconnectStackLock);
		USHORT index = m_DisconnectIndexStack.back();
		m_DisconnectIndexStack.pop_back();
		ReleaseSRWLockExclusive(&m_DisconnectStackLock);

		UINT64 sessionId = m_CurrentID++;
		UINT64 id = CombineIndex(index, sessionId);
		USHORT cindex = GetSessionIndex(id);
		UINT64 csid = GetSessionId(id);

		Session *pSession = Session::Alloc();

		pSession->Init(clientSocket, id);
		m_pArrSession[index] = pSession;

		// wprintf(L"[SYSTEM] ip : %s , port : %d, sessionID : %lld\n", clientAddrBuf, ntohs(clientAddr.sin_port), pSession->m_SessionID);


		// IOCP에 클라소켓 등록
		CreateIoCompletionPort((HANDLE)clientSocket, m_IOCPHandle, (ULONG_PTR)pSession, 0);

		InterlockedIncrement(&pSession->m_ioCount);

		OnAccept(id);

		pSession->PostRecv();

		LONG back = InterlockedDecrement(&pSession->m_ioCount);
		if (back == 0)
			ReleaseSession(pSession);
	}

	return 0;
}
