#pragma once

struct stMissileInfo;

struct stEnemyInfo
{
	// Shape
	char m_chShape;
	
	// hp
	int m_iHp;

	int m_iMissileCount;
	stMissileInfo *m_MissileInfos;

	int m_iMovesCount;
	stPos *m_Moves;
};

// Enemy 구조체
struct stEnemy
{
	// 생존 여부
	bool				m_bIsActive;

	// 현재 좌표
	stPos				m_stPos;

	// SHAPE
	wchar_t				m_chShape;

	// STATS
	int					m_iCurHp;
	int					m_iMaxHp;

	// 미사일 정보
	int					m_iMissileCount = 0;
	stMissileInfo		*m_MissileInfos = nullptr;

	// 이동 경로
	int					m_iMoveCount;
	int					m_iCurMoveIndex;
	stPos				*m_arrMovePos = nullptr;
};

// Enemy 전역변수
// 파일에서 Enemy 수 받아서 할당
extern int g_iEnemyCount;
extern stEnemy *g_arrEnemy;

// enemy templete
extern std::map<char, stEnemyInfo*> g_mapEnemyInfos;