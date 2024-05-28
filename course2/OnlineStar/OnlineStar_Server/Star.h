#pragma once
class Star
{
public:
	void Move(stPos &pos)
	{
		m_Pos.m_iX = pos.m_iX;
		m_Pos.m_iY = pos.m_iY;
	}

public:
	int m_Id;
	stPos m_Pos;

	SOCKET m_ClientSocket;

	char m_Ip[32];
	short m_Port;
};

extern std::map<SOCKET, Star> g_Stars;