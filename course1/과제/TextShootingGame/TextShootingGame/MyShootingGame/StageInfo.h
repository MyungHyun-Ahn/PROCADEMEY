#pragma once

// Stage ���� ����ü
struct stStageInfo
{
	char			*m_chFileName;
	char			*m_arrEnemys;
	int				m_iEnemyCount;
};

extern int g_iStageCount;
extern stStageInfo *g_StageInfos;

extern int g_iCurStage;