#pragma once
#pragma once
class NetworkManager
{
	SINGLE(NetworkManager)
public:
	// ���� ���� ����
	bool Start();

	// Select ȣ�� + ó���ؾ��� �� ����
	bool ReadSelect();
	bool WriteSelect();

	bool SendUnicast(SOCKET sock, char *packet, int size);

private:
	// ��Ŷ ó����
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

