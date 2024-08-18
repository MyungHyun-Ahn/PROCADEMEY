#pragma once

class Player;

inline int CalSectorX(int x)
{
	return (x - RANGE_MOVE_TOP) / SECTOR_SIZE;
}

inline int CalSectorY(int y)
{
	return (y - RANGE_MOVE_LEFT) / SECTOR_SIZE;
}

extern std::unordered_map<INT, Player *> g_Sectors[SECTOR_MAX_Y][SECTOR_MAX_X];