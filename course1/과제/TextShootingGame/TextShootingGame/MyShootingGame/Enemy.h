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
	bool				m_bIsActive;

	// ���� ��ǥ
	stPos				m_stPos;

	// SHAPE
	wchar_t				m_chShape;

	// STATS
	int					m_iCurHp;
	int					m_iMaxHp;

	// �̻��� ����
	int					m_iMissileCount = 0;
	stMissileInfo		*m_MissileInfos = nullptr;

	// �̵� ���
	int					m_iMoveCount;
	int					m_iCurMoveIndex;
	stPos				*m_arrMovePos = nullptr;
};

// Enemy ��������
// ���Ͽ��� Enemy �� �޾Ƽ� �Ҵ�
extern int g_iEnemyCount;
extern stEnemy *g_arrEnemy;

// enemy templete
extern std::map<char, stEnemyInfo*> g_mapEnemyInfos;