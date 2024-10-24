#pragma once

class CNetworkManager
{
	SINGLE(CNetworkManager)
public:
	friend class ProcessPacket;

	// ���� ���� ����
	bool Start();

	// Select ȣ�� + ó���ؾ��� �� ����
	bool Select();

	// �̰� ����ؼ� Send�� �ٷ� ����
	bool SendUnicast(SOCKET sock, char *packet, int size);
	bool RegisterUnicast(CSession *pSession, char *packet, int size);
	bool RegisterBroadcast(CSession *pSession, char *packet, int size);

	void TimeoutCheck();

	// Client disconnect
	bool DisconnectClients();

private:
	// Accept
	bool Accept();
	// Disconnect
	bool Disconnect();

private:
	SOCKET						m_listenSocket;
	fd_set						m_readSet;
	fd_set						m_writeSet;

	std::deque<CSession *>		m_deleteQueue;
};


extern bool g_bShutdown;
extern int g_iCurUserIDValue;
extern CNetworkManager *g_NetworkMgr;

extern int g_AcceptTPS;
extern int g_RecvTPS;
extern int g_SendTPS;
extern int g_SyncCount;

