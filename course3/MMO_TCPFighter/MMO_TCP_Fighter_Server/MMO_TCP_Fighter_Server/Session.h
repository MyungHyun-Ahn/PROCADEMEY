#pragma once

class Session
{
public:
	friend class NetworkManager;
	friend class ProcessPacket;
	friend class ProcessPacketInterface;

	Session() = default;

	Session(int id, SOCKET sock) : m_Id(id), m_ClientSocket(sock)
	{}


private:
	BOOL m_isVaild = TRUE;
	INT m_Id;

	SOCKET m_ClientSocket;
	WCHAR m_szClientIP[16] = { 0 };

	INT m_PrevRecvTime = 0;

	RingBuffer recvBuffer;
	RingBuffer sendBuffer;
};

extern std::unordered_map<INT, Session *> g_Sessions;
extern ObjectPool<Session> g_SessionPool;