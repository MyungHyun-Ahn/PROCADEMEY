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


// listen ���� ����
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

	// �� ���ŷ �������� ��ȯ
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
	// listenSocket ��� - accept ó���� ����
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

	// Recv ó��
	for (int i = 0; i < rset.fd_count; ++i)
	{
		SOCKET targetSock = rset.fd_array[i];

		// listen ������ �� �Ÿ���
		if (targetSock == listenSocket)
			continue;
		char BUF[1024];
		retVal = recv(targetSock, BUF, 1024 * sizeof(char), 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			// ���� ����
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
			// ���� ����
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

		// ��Ŷ ó��
		ProcessPacket(targetSock, BUF, retVal);
	}
	

	return true;
}

// sock���Ը� ����
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

// sock�� ������ ��ο��� ����
bool NetworkManager::SendBroadcast(SOCKET sock, PacketBase *packet)
{
	int retVal;
	int errVal;
	bool allSucc = true;

	for (const std::pair<SOCKET, Star> &star : g_Stars)
	{
		// �ڱ� �ڽ� ����
		if (star.first == sock)
			continue;

		// ��Ŷ ����
		retVal = send(star.first, (char *)packet, sizeof(PacketBase), 0);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			if (errVal != WSAEWOULDBLOCK)
			{
				// printf("send() Error : %d\n", errVal);
				if (errVal == WSAECONNRESET)
				{
					// ���� ���� ť�� �ֱ� - ���� ���� ����
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
	// 0 ���� �Ľ� ����
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
		// �ִ� ���� ���� �ο� �ʰ� 
		return false;
	}

	// �� ���� ����
	Star star;
	star.m_ClientSocket = clientSock;
	star.m_Id = ++g_CurUserIDValue; // ID �� ����
	star.m_Pos = { dfSCREEN_WIDTH / 2,dfSCREEN_HEIGHT / 2};
	InetNtopA(AF_INET, &clientAddr.sin_addr, star.m_Ip, 16);
	star.m_Port = ntohs(clientAddr.sin_port);

	// id �Ҵ� ��Ŷ ����
	{
		PacketAllocID packet;
		packet.Type = (int)PacketCode::AllocID;
		packet.ID = star.m_Id;

		if (!SendUnicast(clientSock, (PacketBase *)&packet))
		{	
			// ������ ���
			// ������ ���´�.
			closesocket(clientSock);
			return false;
		}
	}

	// �ڱ� �ڽ� �� ���� ��Ŷ ����
	{
		PacketCreateStar packet;
		packet.Type = (int)PacketCode::CreateStar;
		packet.ID = star.m_Id;
		packet.X = star.m_Pos.m_iX;
		packet.Y = star.m_Pos.m_iY;

		if (!SendUnicast(clientSock, (PacketBase *)&packet))
		{	// ������ ���
			// ������ ���´�.
			closesocket(clientSock);
			return false;
		}
	}

	// �̹� ���� �ִ� ���� ������ ����
	for (const std::pair<SOCKET, Star> &s : g_Stars)
	{
		Star otherStar = s.second;

		PacketCreateStar packet;
		packet.Type = (int)PacketCode::CreateStar;
		packet.ID = otherStar.m_Id;
		packet.X = otherStar.m_Pos.m_iX;
		packet.Y = otherStar.m_Pos.m_iY;

		if (!SendUnicast(clientSock, (PacketBase *)&packet))
		{	// ������ ���
			// ������ ���´�.
			closesocket(clientSock);
			return false;
		}
	}

	// �� ���� ��Ŷ ��ο��� ���� - ���Դٴ� ��� ��� �˸�
	{
		PacketCreateStar packet;
		packet.Type = (int)PacketCode::CreateStar;
		packet.ID = star.m_Id;
		packet.X = star.m_Pos.m_iX;
		packet.Y = star.m_Pos.m_iY;

		if (!SendBroadcast(NULL, (PacketBase *)&packet))
		{
			// ������ ���� �ִ� ��
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