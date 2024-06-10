#pragma once
class Player
{
public:
	Player(INT id, USHORT x, USHORT y) : m_Id(id), m_X(x), m_Y(y) {}

	bool Move(USHORT x, USHORT y)
	{
		// 예외처리

		return true;
	}

	bool GetDamage(INT damage) { m_Id -= damage; }

private:
	INT m_Id;
	USHORT m_X;
	USHORT m_Y;

	CHAR m_ViewDir;
	INT m_Action;

	SHORT m_Hp = dfMAX_PLAYER_HP;
};

extern unsigned int g_UserId;

// Player 자료구조
extern std::map<INT, Player> m_Players;