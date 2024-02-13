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

// Enemy ����ü
struct stEnemy
{
	// ���� ����
	bool				m_bIsActive = false; // �⺻�� false

	// ���� ��ǥ
	stPos				m_stPos;

	// SHAPE
	char				m_chShape;

	// STATS
	int					m_iCurHp;
	int					m_iMaxHp;

	// �̻��� ���� - m_MissileInfos�� �Ҵ������ϸ� �ȵȴ�.
	// �ܺ� map �� ������ ���� �׳� ������ �� ��
	int					m_iMissileCount = 0;
	stMissileInfo		*m_MissileInfos = nullptr;

	// �̻��� ��Ÿ�� ���� - �̰��� Enemy �� �����Ҵ� �����ؾ� �Ѵ�.
	int					*m_arrMissilePrevTime;

	// �̵� ���
	int					m_iMoveCount;
	int					m_iCurMoveIndex;
	stPos				*m_arrMovePos = nullptr; // �̰� ���� ���⼭ �Ҵ��������� �ʴ´�.
};

// Enemy ��������
// ���Ͽ��� Enemy �� �޾Ƽ� �Ҵ�
extern int g_iEnemyCount;
extern stEnemy *g_arrEnemy;

// enemy template �����ϴ� map - �ߺ� �ε� ����
extern std::map<char, stEnemyInfo*> g_mapEnemyInfos;

// Stage �ε��� ������� Enemy ����
void EnemyInit(void);

// Enemy Logic Update
void EnemyUpdate(void);

// Enemy �̵�
void EnemyMove(void);

// Enemy �̻���
void EnemyAttack(void);

// Enemy�� ���ۿ� ��
void EnemyRender(void);