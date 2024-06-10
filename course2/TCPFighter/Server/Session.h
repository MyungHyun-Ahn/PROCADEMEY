#pragma once

class Session
{
public:
	friend class NetworkManager;

	Session(int id, SOCKET sock) : m_Id(id), clientSocket(sock)
	{}

private:
	INT m_Id;
	SOCKET clientSocket;
	RingBuffer recvBuffer;
	RingBuffer sendBuffer;
};

extern std::map<INT, Session> g_Sesstions;