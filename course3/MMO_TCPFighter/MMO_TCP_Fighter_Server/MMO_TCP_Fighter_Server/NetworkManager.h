#pragma once

class NetworkManager
{
	SINGLE(NetworkManager)
public:
	friend class ProcessPacket;

	// ���� ���� ����
	bool Start();

	// Select ȣ�� + ó���ؾ��� �� ����
	bool Select();

	// �̰� ����ؼ� Send�� �ٷ� ����
	bool SendUnicast(SOCKET sock, char *packet, int size);
	bool RegisterUnicast(Session *pSession, char *packet, int size);
	bool RegisterBroadcast(Session *pSession, char *packet, int size);

	void TimeoutCheck();

	// Client disconnect
	bool DisconnectClients();

private:
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

extern int g_AcceptTPS;
extern int g_RecvTPS;
extern int g_SendTPS;
extern int g_SyncCount;

