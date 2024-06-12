#include "pch.h"
#include "Define.h"
#include "Player.h"
#include "Session.h"
#include "Packet.h"
#include "ProcessPacket.h"
#include "GenPacket.h"
#include "NetworkManager.h"

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

bool NetworkManager::ReadSelect()
{
	int errVal;
	int retVal;

	FD_ZERO(&rset);
	// listenSocket 등록 - accept 처리를 위해
	FD_SET(listenSocket, &rset);

	// 돌면서 등록
	for (auto session : g_Sessions)
	{
		FD_SET(session.second->clientSocket, &rset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	// select()
	retVal = select(0, &rset, NULL, NULL, &t);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"read select() Error : %d\n", errVal);
		return false;
	}

	// 현재 구조에서는 한 프레임에 1명씩만 받을 수 있음
	if (FD_ISSET(listenSocket, &rset))
	{
		Accept();
	}

	for (auto& session : g_Sessions)
	{
		// 읽을게 있는지 체크
		if (FD_ISSET(session.second->clientSocket, &rset))
		{
			Session *s = session.second;

			// recv
			int directSize = s->recvBuffer.DirectEnqueueSize();
			// Rear가 0일 때
			int retVal = recv(s->clientSocket, s->recvBuffer.GetRearPtr(), directSize, 0);
			if (retVal == SOCKET_ERROR)
			{
				int errVal = WSAGetLastError();
				if (errVal != WSAEWOULDBLOCK)
				{
					wprintf(L"recv error : %d\n", errVal);

					// 중복 삽입 방지
					if (s->m_isVaild)
					{
						wprintf(L"Player %d disconnected!\n", s->m_Id);
						deleteQueue.push_back(s);
						s->m_isVaild = FALSE;
					}
					continue;
				}
			}

			s->recvBuffer.MoveRear(retVal);

			if (!ProcessPacket(session.first))
			{
				if (s->m_isVaild)
				{
					wprintf(L"Player %d disconnected!\n", s->m_Id);
					deleteQueue.push_back(s);
					s->m_isVaild = FALSE;
				}
			}
		}
	}

	return true;
}

bool NetworkManager::WriteSelect()
{
	int errVal;
	int retVal;

	FD_ZERO(&wset);

	FD_SET(listenSocket, &wset);

	// 세션 돌면서 등록
	for (auto session : g_Sessions)
	{
		FD_SET(session.second->clientSocket, &wset);
	}

	timeval t;
	t.tv_sec = 0;
	t.tv_usec = 0;

	// select()
	retVal = select(0, NULL, &wset, NULL, &t);
	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		wprintf(L"write select() Error : %d\n", errVal);
		return false;
	}

	for (auto session : g_Sessions)
	{
		// wset에 있는지 체크
		if (FD_ISSET(session.second->clientSocket, &wset))
		{
			// 이때 보낸다.
			Session* s = session.second;

			// 다이렉트로 보낼 수 있는 것은 모두 보낸다.
			int directSize = s->sendBuffer.DirectDequeueSize();

			int retVal = send(s->clientSocket, s->sendBuffer.GetFrontPtr(), directSize, 0);
			if (retVal == SOCKET_ERROR)
			{
				int errVal = WSAGetLastError();
				if (errVal != WSAEWOULDBLOCK)
				{
					wprintf(L"send error : %d\n", errVal);

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

bool NetworkManager::RegisterBroadcast(Session *pSession, char *packet, int size)
{
	for (auto& ses : g_Sessions)
	{
		if (ses.second == pSession)
			continue;

		ses.second->sendBuffer.Enqueue(packet, size);
	}

	return true;
}

bool NetworkManager::ProcessPacket(int sessionId)
{
	// 링버퍼에서 꺼내서
	// 패킷 처리
	Session *curSession = g_Sessions[sessionId];

	while (true)
	{
		int size = curSession->recvBuffer.GetUseSize();
		if (size < sizeof(PacketHeader))
			break;

		PacketHeader header;
		curSession->recvBuffer.Peek((char *)&header, sizeof(PacketHeader));
		if (size < header.bySize + sizeof(PacketHeader))
			break;

		if (!ConsumePacket(curSession, (PACKET_CODE)header.byType))
		{
			// false가 반환 되는 건 잘못된 상황
			return false;
		}

	}

	return true;
}

bool NetworkManager::ConsumePacket(Session *session, PACKET_CODE code)
{

	switch (code)
	{
	case PACKET_CODE::PACKET_CS_MOVE_START:
	{
		PacketCSMoveStart packet;
		session->recvBuffer.Dequeue((char *)&packet, sizeof(PacketCSMoveStart));
		return ProcessPacket::PacketProc_MoveStart(session, (char *)&packet);
	}
	break;

	case PACKET_CODE::PACKET_CS_MOVE_STOP:
	{
		PacketCSMoveStop packet;
		session->recvBuffer.Dequeue((char *)&packet, sizeof(PacketCSMoveStop));
		return ProcessPacket::PacketProc_MoveStop(session, (char *)&packet);
	}
	break;

	case PACKET_CODE::PACKET_CS_ATTACK1:
	{
		PacketCSAttack1 packet;
		session->recvBuffer.Dequeue((char *)&packet, sizeof(PacketCSAttack1));
		return ProcessPacket::PacketProc_Attack1(session, (char *)&packet);
	}
	break;

	case PACKET_CODE::PACKET_CS_ATTACK2:
	{
		PacketCSAttack2 packet;
		session->recvBuffer.Dequeue((char *)&packet, sizeof(PacketCSAttack2));
		return ProcessPacket::PacketProc_Attack2(session, (char *)&packet);
	}
	break;

	case PACKET_CODE::PACKET_CS_ATTACK3:
	{
		PacketCSAttack3 packet;
		session->recvBuffer.Dequeue((char *)&packet, sizeof(PacketCSAttack3));
		return ProcessPacket::PacketProc_Attack3(session, (char *)&packet);
	}
	break;

	case PACKET_CODE::PACKET_CS_SYNC:
	{

	}
	break;

	default:
		break;
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

	// 63명 이상
	if (g_Sessions.size() > 63)
	{
		// 최대 접속 수용 인원 초과 
		closesocket(clientSock);
		return false;
	}


	// 접속 수용
	++g_UserId;
	INT id = g_UserId;

	Session *newSession = new Session(id, clientSock);
	InetNtop(AF_INET, &clientAddr.sin_addr, newSession->szClientIP, 16);

	USHORT ranX = (rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT - 1)) + dfRANGE_MOVE_LEFT + 1;
	USHORT ranY = (rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP - 1)) + dfRANGE_MOVE_TOP + 1;

	Player newPlayer(id, ranX, ranY);

	wprintf(L"Connected # %s\n", newSession->szClientIP);
	wprintf(L"Player ID # %d, x : %d, y : %d\n", id, ranX, ranY);

	// 내 캐릭터 생성 정보 전달
	{
		PacketSCCreateMyCharacter packet;
		makePacketSCCreateMyCharacter(&packet, newPlayer.m_Id, newPlayer.m_Direction, newPlayer.m_X, newPlayer.m_Y, (BYTE)newPlayer.m_Hp);
		newSession->sendBuffer.Enqueue((char *)&packet, sizeof(PacketSCCreateMyCharacter));
	}


	// 현재 들어와있는 유저 정보 전달
	for (auto& player : g_Players)
	{
		Player remainPlayer = player.second;

		PacketSCCreateOtherCharacter packet;
		makePacketSCCreateOtherCharacter(&packet, remainPlayer.m_Id, (CHAR)remainPlayer.m_Direction, remainPlayer.m_X, remainPlayer.m_Y, (CHAR)remainPlayer.m_Hp);
		newSession->sendBuffer.Enqueue((char *)&packet, sizeof(PacketSCCreateMyCharacter));
	}

	// 들어와 있는 모든 유저에게 정보 전달
	for (auto& session : g_Sessions)
	{
		Session* ses = session.second;

		PacketSCCreateOtherCharacter packet;
		makePacketSCCreateOtherCharacter(&packet, newPlayer.m_Id, (CHAR)newPlayer.m_Direction, newPlayer.m_X, newPlayer.m_Y, (CHAR)newPlayer.m_Hp);
		ses->sendBuffer.Enqueue((char *)&packet, sizeof(PacketSCCreateOtherCharacter));
	}

	// 다 보냈으니 map에 Insert
	g_Sessions.insert(std::make_pair(id, newSession));
	g_Players.insert(std::make_pair(id, newPlayer));

	return true;
}

bool NetworkManager::DisconnectClients()
{

	// 남아있는 애들한테 삭제 정보 전송
	for (Session *ses : deleteQueue)
	{
		PacketSCDeleteCharacter packet;
		packet.byCode = dfPACKET_CODE;
		packet.bySize = sizeof(PacketSCDeleteCharacter) - sizeof(PacketHeader);
		packet.byType = (BYTE)PACKET_CODE::PACKET_SC_DELETE_CHARACTER;
		packet.id = ses->m_Id;

		// 들어와 있는 모든 세션에 패킷 인큐
		// 삭제될 세션에 인큐되어도 어차피 아래에서 지워줌
		for (auto &s : g_Sessions)
		{
			s.second->sendBuffer.Enqueue((char *)&packet, sizeof(PacketSCDeleteCharacter));
		}


		closesocket(ses->clientSocket);
		g_Sessions.erase(ses->m_Id);
		g_Players.erase(ses->m_Id);
		// 지워주기
		delete ses;
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