#pragma once

// Player
struct stPlayer;
extern stPlayer g_stPlayer;
extern bool g_bIsGameOver;

// Enemy
// 파일에서 Enemy 수 받아서 할당
struct stEnemyInfo;
struct stEnemy;
extern int g_iEnemyCount;
extern stEnemy *g_arrEnemy;
extern bool g_bIsStageClear;

// enemy template 저장하는 map - 중복 로드 방지
extern std::map<char, stEnemyInfo *> g_mapEnemyInfos;

// Missile
struct stMissile;
extern stMissile g_arrMissile[dfMAX_MISSILE_COUNT];

// Key
struct stKeyInfo;

extern int g_arrVKKey[(int)KEY::END];
extern stKeyInfo g_KeyInfo[(int)KEY::END];

// Timer
struct stTimer;
extern stTimer g_stTimer;

// Scene
struct stScene;

extern stScene g_curScene;

// StageInfo
struct stStageInfo;

extern int g_iStageCount;
extern stStageInfo *g_StageInfos;
extern int g_iCurStage;

// Console
extern HANDLE g_hScreen;
extern char g_szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// Program Exit
extern bool g_bProgramExit;