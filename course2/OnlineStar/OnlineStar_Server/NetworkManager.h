#pragma once
class NetworkManager
{
	SINGLE(NetworkManager)
public:
	// ���� ���� ����
	bool Start();

	// Select ȣ�� + ó���ؾ��� �� ����
	bool Select();

	bool SendUnicast(SOCKET sock, PacketBase* packet);
	bool SendBroadcast(SOCKET sock, PacketBase *packet);

private:
	// ��Ŷ ó����
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