#pragma once

// Stage ���� ����ü
struct stStageInfo
{
	char			*m_chFileName;		// Stage ���� �̸�
	int				m_iEnemyCount;		// �ʿ� �����ϴ� Enemy ��
	int				m_iEnemyTypeCount;	// Enemy ���� ��
	char			*m_arrEnemys;		// Enemy ���� �迭
};

extern int g_iStageCount;
extern stStageInfo *g_StageInfos;

extern int g_iCurStage;