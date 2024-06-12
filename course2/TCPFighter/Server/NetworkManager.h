#pragma once

class NetworkManager
{
	SINGLE(NetworkManager)
public:
	friend class ProcessPacket;

	// ���� ���� ����
	bool Start();

	// Select ȣ�� + ó���ؾ��� �� ����
	bool ReadSelect();
	bool WriteSelect();

	// �̰� ����ؼ� Send�� �ٷ� ����
	bool SendUnicast(SOCKET sock, char* packet, int size);
	bool RegisterBroadcast(Session *pSession, char *packet, int size);

	// Client disconnect
	bool DisconnectClients();

private:
	// ��Ŷ ó����
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

