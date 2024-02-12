#include "pch.h"
#include "Enemy.h"
#include "Console.h"
#include "Missile.h"
#include "TimeManager.h"

int g_iEnemyCount;
stEnemy *g_arrEnemy = nullptr;

std::map<char, stEnemyInfo*> g_mapEnemyInfos;