#pragma once
class Star
{
public:
	void Move(stPos &pos)
	{
		m_Pos.m_iX = pos.m_iX;
		m_Pos.m_iY = pos.m_iY;
	}

	stPos m_Pos;
};

