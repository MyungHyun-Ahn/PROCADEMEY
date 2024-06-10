#include "pch.h"
#include "Define.h"
#include "Session.h"
#include "Player.h"
#include "NetworkManager.h"

bool g_bShutdown = false;
int g_CurUserIDValue = 0;

NetworkManager::NetworkManager()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		throw 1;
	}
}

NetworkManager::~NetworkManager()
{
	WSACleanup();
}

// listen 까지 수행
bool NetworkManager::Start()
{
	int retVal;
	int errVal;

	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		errVal = WSAGetLastError();
		printf("socket() Error : %d\n", errVal);
		return false;
	}

	// bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, SERVER_IP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVER_PORT);
	retVal = bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("bind() Error : %d\n", errVal);
		return false;
	}

	// listen()
	retVal = listen(listenSocket, SOMAXCONN);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("listen() Error : %d\n", errVal);
		return false;
	}

	// 논 블로킹 소켓으로 전환
	u_long on = 1;
	retVal = ioctlsocket(listenSocket, FIONBIO, &on);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("ioctlsocket() Error : %d\n", errVal);
		return false;
	}

	return true;
}

bool NetworkManager::ReadSelect()
{
	int errVal;
	int retVal;

	FD_ZERO(&rset);
	// listenSocket 등록 - accept 처리를 위해
	FD_SET(listenSocket, &rset);

	// 돌면서 등록
	for (auto session : g_Sesstions)
	{
		FD_SET(session.second.clientSocket, &rset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	// select()
	retVal = select(0, &rset, NULL, NULL, &t);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("select() Error : %d\n", errVal);
		return false;
	}

	// 현재 구조에서는 한 프레임에 1명씩만 받을 수 있음
	if (FD_ISSET(listenSocket, &rset))
	{
		Accept();
	}

	for (auto session : g_Sesstions)
	{
		// 읽을게 있는지 체크
		if (FD_ISSET(session.second.clientSocket, &rset))
		{
			Session &s = session.second;

			// recv
			int directSize = s.recvBuffer.DirectEnqueueSize();
			// Rear가 0일 때
			int retVal = recv(s.clientSocket, s.recvBuffer.GetRearPtr(), directSize, 0);
			if (retVal == SOCKET_ERROR)
			{
				int errVal = WSAGetLastError();
				if (errVal != WSAEWOULDBLOCK)
				{
					// 삭제 등록
					continue;
				}
			}

			s.recvBuffer.MoveRear(retVal);

			ProcessPacket(s.clientSocket, session.first);
		}
	}

	return true;
}

bool NetworkManager::WriteSelect()
{
	int errVal;
	int retVal;

	FD_ZERO(&wset);

	// 세션 돌면서 등록
	for (auto session : g_Sesstions)
	{
		FD_SET(session.second.clientSocket, &wset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	// select()
	retVal = select(0, NULL, &wset, NULL, &t);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("select() Error : %d\n", errVal);
		return false;
	}

	for (auto session : g_Sesstions)
	{
		// wset에 있는지 체크
		if (FD_ISSET(session.second.clientSocket, &wset))
		{
			// 이때 보낸다.
			Session &s = session.second;

			// 다이렉트로 보낼 수 있는 것은 모두 보낸다.
			int directSize = s.sendBuffer.DirectDequeueSize();

			int retVal = send(s.clientSocket, s.sendBuffer.GetFrontPtr(), directSize, 0);
			if (retVal == SOCKET_ERROR)
			{
				int errVal = WSAGetLastError();
				if (errVal != WSAEWOULDBLOCK)
				{
					// 연결 종료
					// std::cout << "WriteEvent() : " << errVal << std::endl;
					continue;
				}
			}
			
			if (directSize == retVal)
			{
				s.sendBuffer.Clear();
				continue;
			}

			s.sendBuffer.MoveFront(retVal);
		}
	}

	return true;
}


// sock에게만 전송
bool NetworkManager::SendUnicast(SOCKET sock, char *packet, int size)
{
	int retVal;
	int errVal;

	retVal = send(sock, (char *)packet, size, 0);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("send() Error : %d\n", errVal);
		return false;
	}

	return true;
}

bool NetworkManager::ProcessPacket(SOCKET sock, int sessionId)
{
	// 링버퍼에서 꺼내서
	// 패킷 처리


	return false;
}

bool NetworkManager::Accept()
{
	int errVal;

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSock = accept(listenSocket, (SOCKADDR *)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET)
	{
		errVal = WSAGetLastError();
		printf("accept() Error : %d\n", errVal);
		return false;
	}

	// 63명 이상
	if (g_Sesstions.size() > 63)
	{
		// 최대 접속 수용 인원 초과 
		return false;
	}

	// 접속 수용
	++g_UserId;
	INT id = g_UserId;
	Session newSession(id, clientSock);
	g_Sesstions.insert(std::make_pair(id, newSession));

	int ranX = rand() / (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT - 2) + dfRANGE_MOVE_LEFT;
	int ranY = rand() / (dfRANGE_MOVE_TOP - dfRANGE_MOVE_BOTTOM - 2) + dfRANGE_MOVE_BOTTOM;
	Player newPlayer(id, ranX, ranY);


}

bool NetworkManager::DisconnectClient()
{
	return false;
}

bool NetworkManager::Disconnect()
{
	closesocket(listenSocket);
	return true;
}

NetworkManager *g_NetworkMgr;