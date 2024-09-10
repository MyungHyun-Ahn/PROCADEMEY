#include "pch.h"
#include "Session.h"
#include "CLanServer.h"

CLanServer *g_Server;
std::unordered_map<UINT64, Session *>g_SessionMap;
CRITICAL_SECTION g_SessionMapLock;

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
	InitializeCriticalSection(&g_SessionMapLock);

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

	// // listen 소켓을 IOCP 핸들에 등록
	// auto hIOCPHandle = CreateIoCompletionPort((HANDLE)m_listenSocket, m_IOCPHandle, (UINT32)0, 0);
	// if (nullptr == hIOCPHandle)
	// {
	// 	errVal = WSAGetLastError();
	// 	wprintf(L"[ERROR] listen socket IOCP bind 실패 : %d\n", errVal);
	// 	return false;
	// }

	// // CreateAcceptThread
	// m_AccepterThread = (HANDLE)_beginthreadex(nullptr, 0, AcceptorThreadFunc, nullptr, CREATE_SUSPENDED, nullptr);
	// if (m_AccepterThread == 0)
	// {
	// 	wprintf(L"[ERROR] AccepterThread running fail..\n");
	// 	return false;
	// 
	// }
	// wprintf(L"[SYSTEM] AccepterThread running..\n");

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

	// ResumeThread(m_AccepterThread);
	for (int i = 0; i < createWorkerThreadCount; i++)
	{
		ResumeThread(m_WorkerThreads[i]);
	}

	return true;
}

bool CLanServer::Disconnect(Session *pSession)
{
	pSession->m_IsValid = FALSE;
	closesocket(pSession->m_ClientSocket);
	return true;
}

bool CLanServer::ReleaseSession(Session *pSession)
{
	EnterCriticalSection(&g_SessionMapLock);
	g_SessionMap.erase(pSession->m_SessionID);
	LeaveCriticalSection(&g_SessionMapLock);

	if (pSession->m_IsValid)
		Disconnect(pSession);


	delete pSession;

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
		Session *pSession = nullptr;
		OverlappedEx *lpOverlapped = nullptr;

		retVal = GetQueuedCompletionStatus(m_IOCPHandle, &dwTransferred, (PULONG_PTR)&pSession, (LPOVERLAPPED *)&lpOverlapped, INFINITE);

		// 스레드 종료 (0, 0, 0) 체크 -> 정상 종료
		if (dwTransferred == 0 && pSession == nullptr && lpOverlapped == nullptr)
		{
			break;
		}

		// 소켓 정상 종료
		if (dwTransferred == 0)
		{
			Disconnect(pSession);
			continue;
		}

		if (lpOverlapped->m_Operation == IOOperation::RECV)
		{
			pSession->RecvCompleted(dwTransferred);

			if (!pSession->PostSend())
				ReleaseSession(pSession);

			if (!pSession->PostRecv())
				ReleaseSession(pSession);
		}
		
		if (lpOverlapped->m_Operation == IOOperation::SEND)
		{
			pSession->SendCompleted(dwTransferred);

			if (!pSession->PostSend())
				ReleaseSession(pSession);
		}
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
			// if (errVal == 10038)
			// 	__debugbreak();

			wprintf(L"[ERROR] accept() Error : %d\n", errVal);
			return false;
		}

		WCHAR clientAddrBuf[16] = { 0, };
		InetNtop(AF_INET, &clientAddr.sin_addr, clientAddrBuf, 16);

		// 접속 허용 IP 확인
		if (!OnConnectionRequest(clientAddrBuf, ntohs(clientAddr.sin_port)))
			continue;

		Session *pSession = new Session(clientSocket, ++m_CurrentID);

		// wprintf(L"[SYSTEM] ip : %s , port : %d, sessionID : %lld\n", clientAddrBuf, ntohs(clientAddr.sin_port), pSession->m_SessionID);

		// IOCP에 클라소켓 등록
		CreateIoCompletionPort((HANDLE)clientSocket, m_IOCPHandle, (ULONG_PTR)pSession, 0);
		
		EnterCriticalSection(&g_SessionMapLock);
		g_SessionMap.insert(std::make_pair(pSession->m_SessionID, pSession));
		LeaveCriticalSection(&g_SessionMapLock);

		OnAccept(pSession->m_SessionID);
		pSession->PostRecv();

		//Sleep(10);
	}

	return 0;
}
