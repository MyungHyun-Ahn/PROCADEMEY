#pragma once

class NetworkManager
{
	SINGLE(NetworkManager)
public:
	friend class ProcessPacket;

	// 서버 시작 세팅
	bool Start();

	// Select 호출 + 처리해야할 것 진행
	bool ReadSelect();
	bool WriteSelect();

	// 이걸 사용해서 Send를 바로 진행
	bool SendUnicast(SOCKET sock, char* packet, int size);
	bool RegisterBroadcast(Session *pSession, char *packet, int size);

	// Client disconnect
	bool DisconnectClients();

private:
	// 패킷 처리용
	bool ProcessPacket(int sessionId);
	bool ConsumePacket(Session *session, PACKET_CODE code);

	// Accept
	bool Accept();

	// Disconnect
	bool Disconnect();

private:
	SOCKET listenSocket;
	fd_set rset;
	fd_set wset;

	std::deque<Session *> deleteQueue;
};


extern bool g_bShutdown;
extern int g_CurUserIDValue;
extern NetworkManager *g_NetworkMgr;

