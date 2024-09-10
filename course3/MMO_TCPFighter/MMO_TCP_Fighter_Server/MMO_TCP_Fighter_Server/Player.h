#pragma once

struct SyncDbg
{
	char isStart;
	int sX;
	int sY;
	int cX;
	int cY;
};

class Player
{
public:
	friend class NetworkManager;
	friend class ProcessPacket;
	friend class GameLogic;

	Player() = default;
	Player(INT id, USHORT x, USHORT y) : m_Id(id), m_X(x), m_Y(y), m_Direction((char)MOVE_DIR::MOVE_DIR_LL), m_Action((DWORD)MOVE_DIR::MOVE_DIR_STOP) {}

	bool Move(SHORT x, SHORT y);

	inline bool GetDamage(INT damage) { m_Id -= damage; }

	void MoveSector(int prevY, int prevX, int nowY, int nowX);

private:
	INT m_Id;
	USHORT m_X;
	USHORT m_Y;

	USHORT m_SecX;
	USHORT m_SecY;

	BYTE m_Direction;
	DWORD m_Action;

	SHORT m_Hp = MAX_PLAYER_HP;

#ifdef SYNC_DBG
	std::list<SyncDbg> syncList;
#endif
};

extern unsigned int g_UserId;

// Player �ڷᱸ��
extern std::unordered_map<INT, Player *> g_Players;
extern ObjectPool<Player> g_PlayerPool;
