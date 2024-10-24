#pragma once

class CSession
{
public:
	friend class CNetworkManager;
	friend class ProcessPacket;
	friend class ProcessPacketInterface;

	CSession() = default;

	CSession(int id, SOCKET sock) : m_Id(id), m_ClientSocket(sock)
	{}


private:
	BOOL m_isVaild = TRUE;
	INT m_Id;

	SOCKET m_ClientSocket;
	WCHAR m_szClientIP[16] = { 0 };

	INT m_PrevRecvTime = 0;

	CRingBuffer recvBuffer;
	CRingBuffer sendBuffer;
};

extern std::unordered_map<INT, CSession *> g_Sessions;
extern CObjectPool<CSession> g_SessionPool;