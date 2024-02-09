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

	int m_iDamage;

	// ���� ��Ÿ�� : ms ����
	int m_iCoolTime;
	// ���� ��ų ��� �ð� : �⺻�� 0
	unsigned __int64 m_iPrevSkillTime = 0;
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

void EnemyGetDamage(stEnemy &enemy, int damage);

void EnemyDraw(void);