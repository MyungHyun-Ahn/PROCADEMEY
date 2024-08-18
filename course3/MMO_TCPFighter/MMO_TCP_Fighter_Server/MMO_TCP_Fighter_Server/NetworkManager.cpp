#include "pch.h"
#include "Define.h"
#include "Player.h"
#include "Session.h"
#include "DefinePacket.h"
#include "GenPacket.h"
#include "NetworkManager.h"
#include "Sector.h"
#include "ProcessPacket.h"

bool g_bShutdown = false;
int g_CurUserIDValue = 0;

NetworkManager::NetworkManager() : listenSocket(INVALID_SOCKET)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		throw 1;
	}

	printf("WSAStartup #\n");
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
		wprintf(L"bind() Error : %d\n", errVal);
		return false;
	}

	wprintf(L"Bind OK # Port : %d\n", SERVER_PORT);

	// listen()
	retVal = listen(listenSocket, SOMAXCONN);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"listen() Error : %d\n", errVal);
		return false;
	}

	wprintf(L"Listen OK #\n");

	// 논 블로킹 소켓으로 전환
	u_long on = 1;
	retVal = ioctlsocket(listenSocket, FIONBIO, &on);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"ioctlsocket() Error : %d\n", errVal);
		return false;
	}

	return true;
}

bool NetworkManager::Select()
{
	int errVal;
	int retVal;

	int loopCount = 0;

	auto startIt = g_Sessions.begin();
	auto prevStartIt = startIt;
	auto endIt = startIt;
	std::advance(endIt, 63);

	INT time = timeGetTime();

	while (true)
	{
		FD_ZERO(&rset);
		FD_ZERO(&wset);

		// listenSocket 등록 - accept 처리를 위해
		FD_SET(listenSocket, &rset);

		startIt = prevStartIt;

		int sesCount = 0;

		// 돌면서 등록
		for (; startIt != g_Sessions.end(); ++startIt)
		{
			++sesCount;

			Session *ses = startIt->second;

			if (time - ses->m_PrevRecvTime > NETWORK_PACKET_RECV_TIMEOUT && ses->m_isVaild)
			{
				deleteQueue.push_back(ses);
				ses->m_isVaild = FALSE;
				continue;
			}

			FD_SET(ses->m_ClientSocket, &rset);

			if (ses->sendBuffer.GetUseSize() > 0)
				FD_SET(ses->m_ClientSocket, &wset);


			if (sesCount == 63)
				break;
		}

		timeval t{ 0,0 };

		// select()
		retVal = select(0, &rset, &wset, NULL, &t);
		if (retVal == SOCKET_ERROR)
		{
			errVal = WSAGetLastError();
			wprintf(L"read select() Error : %d\n", errVal);
			__debugbreak();
			return false;
		}

		// 신규 유저
		if (FD_ISSET(listenSocket, &rset))
		{
			Accept();
		}

		startIt = prevStartIt;

		for (; startIt != endIt; ++startIt)
		{
			if (startIt == g_Sessions.end())
				break;

			// 읽을게 있는지 체크
			if (FD_ISSET(startIt->second->m_ClientSocket, &rset))
			{
				Session *s = startIt->second;

				// recv
				int directSize = s->recvBuffer.DirectEnqueueSize();
				// Rear가 0일 때
				int retVal = recv(s->m_ClientSocket, s->recvBuffer.GetRearPtr(), directSize, 0);
				if (retVal == SOCKET_ERROR)
				{
					int errVal = WSAGetLastError();
					if (errVal != WSAEWOULDBLOCK)
					{
						// wprintf(L"recv error : %d\n", errVal);

						// 중복 삽입 방지
						if (s->m_isVaild)
						{
							// wprintf(L"Player %d disconnected!\n", s->m_Id);
							deleteQueue.push_back(s);
							s->m_isVaild = FALSE;
						}
						continue;
					}
				}
				if (retVal == 0)
				{
					// 중복 삽입 방지
					if (s->m_isVaild)
					{
						//wprintf(L"Player %d disconnected!\n", s->m_Id);
						deleteQueue.push_back(s);
						s->m_isVaild = FALSE;
					}
					continue;
				}

				s->m_PrevRecvTime = timeGetTime();

				s->recvBuffer.MoveRear(retVal);

				if (!g_pProcessPacket->Process(startIt->first))
				{
					if (s->m_isVaild)
					{
						// wprintf(L"Player %d disconnected!\n", s->m_Id);
						deleteQueue.push_back(s);
						s->m_isVaild = FALSE;
					}
				}
			}

			// wset에 있는지 체크
			if (FD_ISSET(startIt->second->m_ClientSocket, &wset))
			{
				// 이때 보낸다.
				Session *s = startIt->second;

				// 다이렉트로 보낼 수 있는 것은 모두 보낸다.
				int directSize = s->sendBuffer.DirectDequeueSize();

				int retVal = send(s->m_ClientSocket, s->sendBuffer.GetFrontPtr(), directSize, 0);
				if (retVal == SOCKET_ERROR)
				{
					int errVal = WSAGetLastError();
					if (errVal != WSAEWOULDBLOCK)
					{
						// wprintf(L"send error : %d\n", errVal);

						// 중복 삽입 방지
						if (s->m_isVaild)
						{
							deleteQueue.push_back(s);
							s->m_isVaild = FALSE;
						}

						continue;
					}
				}

				if (directSize == retVal)
				{
					s->sendBuffer.Clear();
					continue;
				}

				s->sendBuffer.MoveFront(retVal);
			}
		}

		for (int i = 0; i < 63; i++)
		{
			++prevStartIt;
			++endIt;
		}

		if (startIt == g_Sessions.end())
			break;

		loopCount++;
	}

	return true;
}

// sock에게만 전송 - 바로 바로 보낼 거 전송
bool NetworkManager::SendUnicast(SOCKET sock, char *packet, int size)
{
	int retVal;
	int errVal;

	retVal = send(sock, (char *)packet, size, 0);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"send() Error : %d\n", errVal);
		return false;
	}

	return true;
}

bool NetworkManager::RegisterUnicast(Session *pSession, char *packet, int size)
{
	int ret = pSession->sendBuffer.Enqueue(packet, size);
	if (ret == -1)
		return false;

	return true;
}

bool NetworkManager::RegisterBroadcast(Session *pSession, char *packet, int size)
{


	int startY = g_Players[pSession->m_Id]->m_Y - SECTOR_VIEW_START;
	int startX = g_Players[pSession->m_Id]->m_X - SECTOR_VIEW_START;

	for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
	{
		for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
		{
			if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
				continue;

			auto &s = g_Sectors[startY + y][startX + x];
			for (auto &player : g_Sectors[startY + y][startX + x])
			{
				if (player.second->m_Id == pSession->m_Id)
					continue;

				g_Sessions[player.first]->sendBuffer.Enqueue(packet, size);
			}
		}
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
		wprintf(L"accept() Error : %d\n", errVal);
		return false;
	}

	// 접속 수용
	++g_UserId;
	INT id = g_UserId;

	Session *newSession = g_SessionPool.Alloc();
	newSession->m_Id = id;
	newSession->m_ClientSocket = clientSock;
	newSession->m_PrevRecvTime = timeGetTime();

	InetNtop(AF_INET, &clientAddr.sin_addr, newSession->m_szClientIP, 16);

	USHORT ranX = (rand() % (RANGE_MOVE_RIGHT - RANGE_MOVE_LEFT - 1)) + RANGE_MOVE_LEFT + 1;
	USHORT ranY = (rand() % (RANGE_MOVE_BOTTOM - RANGE_MOVE_TOP - 1)) + RANGE_MOVE_TOP + 1;

	Player *newPlayer = g_PlayerPool.Alloc();
	newPlayer->m_Id = id;
	newPlayer->m_Y = ranY;
	newPlayer->m_X = ranX;
	newPlayer->m_Action = (DWORD)MOVE_DIR::MOVE_DIR_STOP;
	newPlayer->m_Direction = (DWORD)MOVE_DIR::MOVE_DIR_RR;

	// 내 캐릭터 생성 정보 전달
	GenPacket::makePacketSCCreateMyCharacter(false, newSession, newPlayer->m_Id, newPlayer->m_Direction, newPlayer->m_X, newPlayer->m_Y, (BYTE)newPlayer->m_Hp);

	int secY = CalSectorY(ranY);
	int secX = CalSectorX(ranX);
	
	newPlayer->m_SecY = secY;
	newPlayer->m_SecX = secX;

	// wprintf(L"Connected # %s\n", newSession->szClientIP);
	// wprintf(L"Player ID # %d, x : %d, y : %d\n", id, ranX, ranY);
	// wprintf(L"Sector # %d, x : %d, y : %d\n", id, secX, secY);

	int startY = secY - SECTOR_VIEW_START;
	int startX = secX - SECTOR_VIEW_START;

	for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
	{
		for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
		{
			if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
				continue;

			auto &s = g_Sectors[startY + y][startX + x];
			for (auto player : g_Sectors[startY + y][startX + x])
			{
				Player *remainPlayer = player.second;

				// unicast
				GenPacket::makePacketSCCreateOtherCharacter(false, newSession, remainPlayer->m_Id, (CHAR)remainPlayer->m_Direction, remainPlayer->m_X, remainPlayer->m_Y, (CHAR)remainPlayer->m_Hp);

				GenPacket::makePacketSCMoveStart(false, newSession, remainPlayer->m_Id, (CHAR)remainPlayer->m_Action, remainPlayer->m_X, remainPlayer->m_Y);

				Session *ses = g_Sessions[player.first];
				GenPacket::makePacketSCCreateOtherCharacter(false, ses, newPlayer->m_Id, (CHAR)newPlayer->m_Direction, newPlayer->m_X, newPlayer->m_Y, (CHAR)newPlayer->m_Hp);
			}
		}
	}

	g_Sectors[secY][secX].insert(std::make_pair(id, newPlayer));

	// 다 보냈으니 map에 Insert
	g_Sessions.insert(std::make_pair(id, newSession));
	g_Players.insert(std::make_pair(id, newPlayer));

	return true;
}

void NetworkManager::TimeoutCheck()
{
	INT time = timeGetTime();
	for (auto &ses : g_Sessions)
	{
		if (time - ses.second->m_PrevRecvTime > NETWORK_PACKET_RECV_TIMEOUT)
		{
			if (ses.second->m_isVaild)
			{
				deleteQueue.push_back(ses.second);
				ses.second->m_isVaild = FALSE;
			}
		}
	}
}

bool NetworkManager::DisconnectClients()
{

	// 남아있는 애들한테 삭제 정보 전송
	for (Session *ses : deleteQueue)
	{
		// 들어와 있는 모든 세션에 패킷 인큐
		// 삭제될 세션에 인큐되어도 어차피 아래에서 지워줌

		// Broadcast
		Player *delPlayer = g_Players[ses->m_Id];

		int secY = delPlayer->m_SecY;
		int secX = delPlayer->m_SecX;

		/*
		if (g_Sectors[secY][secX].find(delPlayer->m_Id) == g_Sectors[secY][secX].end())
			__debugbreak();
		*/

		g_Sectors[secY][secX].erase(delPlayer->m_Id);
		g_Players.erase(delPlayer->m_Id);
		g_Sessions.erase(ses->m_Id);

		int startY = secY - SECTOR_VIEW_START;
		int startX = secX - SECTOR_VIEW_START;

		for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
		{
			for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
			{
				if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
					continue;

				for (auto player : g_Sectors[startY + y][startX + x])
				{
					Player *remainPlayer = player.second;

					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[remainPlayer->m_Id], ses->m_Id);
				}
			}
		}

		closesocket(ses->m_ClientSocket);
		g_SessionPool.Free(ses);
		g_PlayerPool.Free(delPlayer);
	}

	deleteQueue.clear();
	return TRUE;
}

bool NetworkManager::Disconnect()
{
	closesocket(listenSocket);
	return true;
}

NetworkManager *g_NetworkMgr;