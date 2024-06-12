#pragma once
class Player
{
public:
	friend class NetworkManager;
	friend class ProcessPacket;
	friend class GameLogic;

	Player() = default;
	Player(INT id, USHORT x, USHORT y) : m_Id(id), m_X(x), m_Y(y), m_Direction((char)MOVE_DIR::MOVE_DIR_LL), m_Action((DWORD)MOVE_DIR::MOVE_DIR_STOP) {}

	bool Move(SHORT x, SHORT y)
	{
		bool flagX = true;
		bool flagY = true;

		if (m_X + x <= dfRANGE_MOVE_LEFT || m_X + x >= dfRANGE_MOVE_RIGHT)
		{
			flagX = false;
		}

		if (m_Y + y <= dfRANGE_MOVE_TOP || m_Y + y >= dfRANGE_MOVE_BOTTOM)
		{
			flagY = false;
		}
		
		if (flagX && flagY)
		{
			m_X += x;
			m_Y += y;
			wprintf(L"Player id %d Move x : %d, y : %d\n", m_Id, m_X, m_Y);

			return true;
		}

		return false;
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
extern std::map<INT, Player> g_Players;