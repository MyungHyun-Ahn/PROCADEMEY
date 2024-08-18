#include "pch.h"
#include "Define.h"
#include "Session.h"
#include "Player.h"
#include "Sector.h"

std::unordered_map<INT, Player *> g_Sectors[(RANGE_MOVE_BOTTOM - RANGE_MOVE_TOP) / SECTOR_SIZE][(RANGE_MOVE_RIGHT - RANGE_MOVE_LEFT) / SECTOR_SIZE];