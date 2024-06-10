#pragma once
#pragma once
class NetworkManager
{
	SINGLE(NetworkManager)
public:
	// 서버 시작 세팅
	bool Start();

	// Select 호출 + 처리해야할 것 진행
	bool ReadSelect();
	bool WriteSelect();

	bool SendUnicast(SOCKET sock, char *packet, int size);

private:
	// 패킷 처리용
	bool ProcessPacket(SOCKET sock, int sessionId);

	// Accept
	bool Accept();

	// Client disconnect
	bool DisconnectClient();

	// Disconnect
	bool Disconnect();

private:
	SOCKET listenSocket;
	fd_set rset;
	fd_set wset;

	std::deque<SOCKET> deleteQueue;
};


extern bool g_bShutdown;
extern int g_CurUserIDValue;
extern NetworkManager *g_NetworkMgr;

