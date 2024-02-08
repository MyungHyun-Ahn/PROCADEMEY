#pragma once

struct stPos;

struct stEnemy
{
	// 생존 여부
	bool m_bIsActive;

	int m_iCurHp;
	int m_iMaxHp;

	stPos m_stPos;

	wchar_t m_chShape;

	// 이동 경로
	int m_iPosCount;
	int m_iCurPos;
	stPos *m_ArrPos;
};

// 몬스터 동적할당
// 파일에서 몬스터 마리수 받아서 배치
extern int g_iEnemyCount;
extern stEnemy *g_arrEnemy;

void EnemyInit(int enemyCount);

void EnemyReset(void);

void EnemyUpdate(void);

void EnemyMove(void);

void EnemyAttack(void);

void EnemyDraw(void);