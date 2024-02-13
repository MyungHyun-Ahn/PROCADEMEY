#include "pch.h"
#include "GlobalVariable.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "StageInfo.h"
#include "Player.h"
#include "Missile.h"
#include "Enemy.h"

// Player
stPlayer g_stPlayer;
bool g_bIsGameOver = false;

// Enemy
int g_iEnemyCount;
stEnemy *g_arrEnemy = nullptr;
bool g_bIsStageClear = false;
std::map<char, stEnemyInfo *> g_mapEnemyInfos;

// Missile
stMissile g_arrMissile[dfMAX_MISSILE_COUNT];

// Key
int g_arrVKKey[(int)KEY::END] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	'Q', 'W', 'E', 'R',
	'D', 'F',

	VK_CONTROL,
	VK_SPACE,
	VK_ESCAPE
};

stKeyInfo g_KeyInfo[(int)KEY::END];

// Timer
stTimer g_stTimer;

// Scene
stScene g_curScene;

// StageInfo
int g_iStageCount;
stStageInfo *g_StageInfos;
int g_iCurStage = 0;

// Console
HANDLE g_hScreen;
char g_szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];