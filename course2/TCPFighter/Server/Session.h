#pragma once

class Session
{
public:
	friend class NetworkManager;
	friend class ProcessPacket;

	Session(int id, SOCKET sock) : m_Id(id), clientSocket(sock)
	{}


private:
	BOOL m_isVaild = TRUE;
	INT m_Id;
	SOCKET clientSocket;
	WCHAR szClientIP[16] = { 0 };

	RingBuffer recvBuffer;
	RingBuffer sendBuffer;
};

extern std::map<INT, Session *> g_Sessions;