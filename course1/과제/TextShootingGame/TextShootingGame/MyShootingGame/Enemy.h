#pragma once

struct stMissileInfo;

struct stEnemyInfo
{
	// Shape
	char					m_chShape;
	
	// hp
	int						m_iHp;
	int						m_iSpeed;

	int						m_iMissileCount;
	stMissileInfo			*m_MissileInfos;

	int						m_iMovesCount;
	stPos					*m_Moves;
};

// Enemy ����ü
struct stEnemy
{
	// ���� ����
	bool					m_bIsActive = false; // �⺻�� false

	// ���� ��ǥ
	stPos					m_stPos;

	// SHAPE
	char					m_chShape;

	// STATS
	int						m_iCurHp;
	int						m_iMaxHp;
	int						m_iSpeed;

	// �̻��� ���� - m_MissileInfos�� �Ҵ������ϸ� �ȵȴ�.
	// �ܺ� map �� ������ ���� �׳� ������ �� ��
	int						m_iMissileCount = 0;
	stMissileInfo			*m_MissileInfos = nullptr;

	// �̻��� ��Ÿ�� ���� - �̰��� Enemy �� �����Ҵ� �����ؾ� �Ѵ�.
	unsigned int			*m_arrMissilePrevTime;

	// �̵� ���
	int						m_iMoveCount;
	int						m_iCurMoveIndex;
	double					m_dCurMoveIndex;
	stPos					*m_arrMovePos = nullptr; // �̰� ���� ���⼭ �Ҵ��������� �ʴ´�.
};

// Stage �ε��� ������� Enemy ����
void EnemyInit(void);

// Enemy ����
void EnemyRelease(void);

// Enemy Logic Update
void EnemyUpdate(void);

// Enemy �̵�
void EnemyMove(void);

// Enemy �̻���
void EnemyAttack(void);

// Enemy Get Damage
void EnemyGetDamage(int enemyIndex, int damage);

void EnemyClearCheck(void);

// Enemy�� ���ۿ� ��
void EnemyRender(void);