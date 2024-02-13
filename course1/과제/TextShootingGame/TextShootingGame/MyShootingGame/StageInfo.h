#pragma once

// Stage 정보 구조체
struct stStageInfo
{
	char			*m_chFileName;		// Stage 파일 이름
	int				m_iEnemyCount;		// 맵에 존재하는 Enemy 수
	int				m_iEnemyTypeCount;	// Enemy 종류 수
	char			*m_arrEnemys;		// Enemy 저장 배열
};

extern int g_iStageCount;
extern stStageInfo *g_StageInfos;

extern int g_iCurStage;