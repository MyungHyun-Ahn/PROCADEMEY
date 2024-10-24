#include "pch.h"
#include "Define.h"
#include "CSession.h"
#include "CPlayer.h"
#include "Sector.h"

std::unordered_map<INT, CPlayer *> g_Sectors[(RANGE_MOVE_BOTTOM - RANGE_MOVE_TOP) / SECTOR_SIZE][(RANGE_MOVE_RIGHT - RANGE_MOVE_LEFT) / SECTOR_SIZE];