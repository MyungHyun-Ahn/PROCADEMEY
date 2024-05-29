#include "pch.h"
#include "Define.h"
#include "Packet.h"
#include "Star.h"
#include "NetworkManager.h"

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

bool NetworkManager::Select()
{
	int errVal;
	int retVal;

	FD_ZERO(&rset);
	// listenSocket 등록 - accept 처리를 위해
	FD_SET(listenSocket, &rset);

	for (auto &s : g_Stars)
	{
		FD_SET(s.first, &rset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 20;

	// select()
	retVal = select(0, &rset, NULL, NULL, &t);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("select() Error : %d\n", errVal);
		return false;
	}

	if (FD_ISSET(listenSocket, &rset))
	{
		Accept();
	}

	// Recv 처리
	for (int i = 0; i < rset.fd_count; ++i)
	{
		SOCKET targetSock = rset.fd_array[i];

		// listen 소켓일 시 거르기
		if (targetSock == listenSocket)
			continue;
		char BUF[1024];
		retVal = recv(targetSock, BUF, 1024 * sizeof(char), 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			// 연결 끊기
			{
				PacketDestroyStar packet;
				packet.Type = (int)PacketCode::DestroyStar;
				packet.ID = g_Stars[targetSock].m_Id;
				SendBroadcast(targetSock, (PacketBase *)&packet);
			}

			g_Stars.erase(targetSock);
			closesocket(targetSock);
			// printf("recv() Error : %d\n", errVal);
			continue;
		}
		else if (retVal == 0)
		{
			errVal = WSAGetLastError();
			// 연결 끊기
			{
				PacketDestroyStar packet;
				packet.Type = (int)PacketCode::DestroyStar;
				packet.ID = g_Stars[targetSock].m_Id;
				SendBroadcast(targetSock, (PacketBase *)&packet);
			}

			g_Stars.erase(targetSock);
			closesocket(targetSock);
			continue;
		}

		// 패킷 처리
		ProcessPacket(targetSock, BUF, retVal);
	}
	

	return true;
}

// sock에게만 전송
bool NetworkManager::SendUnicast(SOCKET sock, PacketBase* packet)
{
	int retVal;
	int errVal;

	retVal = send(sock, (char *)packet, sizeof(PacketBase), 0);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		printf("send() Error : %d\n", errVal);
		return false;
	}

	return true;
}

// sock을 제외한 모두에게 전송
bool NetworkManager::SendBroadcast(SOCKET sock, PacketBase *packet)
{
	int retVal;
	int errVal;
	bool allSucc = true;

	for (const std::pair<SOCKET, Star> &star : g_Stars)
	{
		// 자기 자신 제외
		if (star.first == sock)
			continue;

		// 패킷 전송
		retVal = send(star.first, (char *)packet, sizeof(PacketBase), 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			if (errVal != WSAEWOULDBLOCK)
			{
				// printf("send() Error : %d\n", errVal);
				if (errVal == WSAECONNRESET)
				{
					// 연결 끊기 큐에 넣기 - 당장 끊지 않음
					deleteQueue.push_back(star.first);
					allSucc = false;
				}
			}
		}
	}

	return allSucc;
}

bool NetworkManager::ProcessPacket(SOCKET sock, char* packet, int packetSize)
{
	// 0 부터 파싱 시작
	int curPos = 0;
	while (curPos < packetSize)
	{
		PacketBase *packetBase = (PacketBase *)(packet + curPos);
		PacketCode code = (PacketCode)packetBase->Type;
		switch (code)
		{
		case PacketCode::MoveStar:
		{
			PacketMoveStar *packet = (PacketMoveStar *)packetBase;
			g_Stars[sock].m_Pos.m_iX = packet->X;
			g_Stars[sock].m_Pos.m_iY = packet->Y;

			if (!SendBroadcast(sock, (PacketBase *)packet))
			{
				DisconnectClient();
			}
		}
		break;

		default:
		{

		}
		}
		curPos += 16;
	}

	return true;
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

	if (g_Stars.size() > 64)
	{
		// 최대 접속 수용 인원 초과 
		return false;
	}

	// 별 정보 세팅
	Star star;
	star.m_ClientSocket = clientSock;
	star.m_Id = ++g_CurUserIDValue; // ID 값 증가
	star.m_Pos = { dfSCREEN_WIDTH / 2,dfSCREEN_HEIGHT / 2};
	InetNtopA(AF_INET, &clientAddr.sin_addr, star.m_Ip, 16);
	star.m_Port = ntohs(clientAddr.sin_port);

	// id 할당 패킷 전송
	{
		PacketAllocID packet;
		packet.Type = (int)PacketCode::AllocID;
		packet.ID = star.m_Id;

		if (!SendUnicast(clientSock, (PacketBase *)&packet))
		{	
			// 실패한 경우
			// 연결을 끊는다.
			closesocket(clientSock);
			return false;
		}
	}

	// 자기 자신 별 생성 패킷 전송
	{
		PacketCreateStar packet;
		packet.Type = (int)PacketCode::CreateStar;
		packet.ID = star.m_Id;
		packet.X = star.m_Pos.m_iX;
		packet.Y = star.m_Pos.m_iY;

		if (!SendUnicast(clientSock, (PacketBase *)&packet))
		{	// 실패한 경우
			// 연결을 끊는다.
			closesocket(clientSock);
			return false;
		}
	}

	// 이미 들어와 있는 유저 데이터 전송
	for (const std::pair<SOCKET, Star> &s : g_Stars)
	{
		Star otherStar = s.second;

		PacketCreateStar packet;
		packet.Type = (int)PacketCode::CreateStar;
		packet.ID = otherStar.m_Id;
		packet.X = otherStar.m_Pos.m_iX;
		packet.Y = otherStar.m_Pos.m_iY;

		if (!SendUnicast(clientSock, (PacketBase *)&packet))
		{	// 실패한 경우
			// 연결을 끊는다.
			closesocket(clientSock);
			return false;
		}
	}

	// 별 생성 패킷 모두에서 전송 - 들어왔다는 사실 모두 알림
	{
		PacketCreateStar packet;
		packet.Type = (int)PacketCode::CreateStar;
		packet.ID = star.m_Id;
		packet.X = star.m_Pos.m_iX;
		packet.Y = star.m_Pos.m_iY;

		if (!SendBroadcast(NULL, (PacketBase *)&packet))
		{
			// 삭제할 것이 있는 것
			DisconnectClient();
		}
	}

	g_Stars[clientSock] = star;

	return true;
}

bool NetworkManager::DisconnectClient()
{
	for (SOCKET sock : deleteQueue)
	{
		PacketDestroyStar packet;
		packet.Type = (int)PacketCode::DestroyStar;
		packet.ID = g_Stars[sock].m_Id;
		SendBroadcast(sock, (PacketBase *)&packet);
		closesocket(sock);
	}

	deleteQueue.clear();

	return true;
}

bool NetworkManager::Disconnect()
{
	closesocket(listenSocket);
	return true;
}

NetworkManager *g_NetworkMgr;