#pragma once
class NetworkManager
{
	SINGLE(NetworkManager)
public:
	// 서버 시작 세팅
	bool Start();

	// Select 호출 + 처리해야할 것 진행
	bool Select();

	bool SendUnicast(SOCKET sock, PacketBase* packet);
	bool SendBroadcast(SOCKET sock, PacketBase *packet);

private:
	// 패킷 처리용
	bool ProcessPacket(SOCKET sock, char* packet, int packetSize);

	// Accept
	bool Accept();

	// Client disconnect
	bool DisconnectClient();

	// Disconnect
	bool Disconnect();

private:
	SOCKET listenSocket;
	fd_set rset;

	std::deque<SOCKET> deleteQueue;
};

extern int g_CurUserIDValue;
extern NetworkManager *g_NetworkMgr;