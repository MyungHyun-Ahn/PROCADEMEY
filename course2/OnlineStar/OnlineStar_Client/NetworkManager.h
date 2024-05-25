#pragma once
class NetworkManager
{
	SINGLE(NetworkManager)

public:
	bool Connect();
	void CloseSocket();
	void RegisterIP(const CHAR *ip);
	bool SetNonBlockingOpt();

	bool Select();
	bool Send(PacketBase *packet);
	bool Recv();

private:
	SOCKET serverSocket;
	CHAR serverIP[16];
	FD_SET rset;
};

extern NetworkManager *g_NetworkMgr;

extern int g_BufferSize;
extern char g_Buffer[RECV_BUFFER_SIZE];
