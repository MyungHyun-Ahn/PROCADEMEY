#pragma once
class Player
{
public:
	Player(INT id, USHORT x, USHORT y) : m_Id(id), m_X(x), m_Y(y), m_Direction((char)MOVE_DIR::MOVE_DIR_LL), m_Action((DWORD)ACTIONS::NONE) {}

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

	BYTE m_Direction;
	DWORD m_Action;

	SHORT m_Hp = dfMAX_PLAYER_HP;
};

extern unsigned int g_UserId;

// Player 자료구조
extern std::map<INT, Player> m_Players;