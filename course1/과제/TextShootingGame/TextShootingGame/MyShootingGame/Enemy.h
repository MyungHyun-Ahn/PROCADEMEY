#pragma once

struct stPos;

struct stEnemy
{
	// ���� ����
	bool m_bIsActive;

	int m_iCurHp;
	int m_iMaxHp;

	stPos m_stPos;

	wchar_t m_chShape;

	// �̵� ���
	int m_iPosCount;
	int m_iCurPos;
	stPos *m_ArrPos;
};

// ���� �����Ҵ�
// ���Ͽ��� ���� ������ �޾Ƽ� ��ġ
extern int g_iEnemyCount;
extern stEnemy *g_arrEnemy;

void EnemyInit(int enemyCount);

void EnemyReset(void);

void EnemyUpdate(void);

void EnemyMove(void);

void EnemyAttack(void);

void EnemyDraw(void);