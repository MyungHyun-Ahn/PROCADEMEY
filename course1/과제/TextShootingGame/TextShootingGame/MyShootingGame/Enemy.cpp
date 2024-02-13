#include "pch.h"
#include "Enemy.h"
#include "Console.h"
#include "Missile.h"
#include "TimeManager.h"
#include "StageInfo.h"
#include "SceneManager.h"

int g_iEnemyCount;
stEnemy *g_arrEnemy = nullptr;

std::map<char, stEnemyInfo*> g_mapEnemyInfos;

void EnemyInit(void)
{
	g_iEnemyCount = g_StageInfos[g_iCurStage].m_iEnemyCount;

	// Enemy 수 만큼 할당
	g_arrEnemy = (stEnemy *)malloc(sizeof(stEnemy) * g_iEnemyCount);
	if (g_arrEnemy == nullptr)
		return;
	
	int enemyIndex = 0;
	for (int y = 0; y < dfSCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < dfSCREEN_WIDTH; x++)
		{
			// 공백 혹은 NULL 문자 건너뛰기
			if (g_curScene.m_szConsoleBuffer[y][x] == ' ' || g_curScene.m_szConsoleBuffer[y][x] == '\0')
				continue;

			// 유효한 Enemy 문자일 경우 map에서 찾는다.
			auto it = g_mapEnemyInfos.find(g_curScene.m_szConsoleBuffer[y][x]);

			// 잘못된 경우 건너뛰기
			if (it == g_mapEnemyInfos.end())
				continue;

			// info 를 기준으로 Enemy 생성
			stEnemyInfo *info = it->second;

			// 기본 정보 세팅
			g_arrEnemy[enemyIndex].m_bIsActive = true;							// 활성화
			g_arrEnemy[enemyIndex].m_stPos = stPos{ y, x };						// 현재 좌표
			g_arrEnemy[enemyIndex].m_chShape = info->m_chShape;					// Shape
			g_arrEnemy[enemyIndex].m_iCurHp = info->m_iHp;						// Hp
			g_arrEnemy[enemyIndex].m_iMaxHp = info->m_iHp;						// Hp

			// 미사일 정보 세팅
			g_arrEnemy[enemyIndex].m_iMissileCount = info->m_iMissileCount;		// 미사일 종류
			g_arrEnemy[enemyIndex].m_MissileInfos = info->m_MissileInfos;		// 가져다 쓸 것 - 얕은 복사

			g_arrEnemy[enemyIndex].m_arrMissilePrevTime = (int *)malloc(sizeof(int) * info->m_iMissileCount);
			if (g_arrEnemy[enemyIndex].m_arrMissilePrevTime == nullptr)
				return;
			// 첫 발사 타이밍을 기록
			for (int i = 0; i < info->m_iMissileCount; i++)
			{
				g_arrEnemy[enemyIndex].m_arrMissilePrevTime[i] = info->m_MissileInfos[i].m_iFirstTime - info->m_MissileInfos[i].m_iCooltime;
			}

			// 이동 정보 세팅
			g_arrEnemy[enemyIndex].m_iMoveCount = info->m_iMovesCount;
			g_arrEnemy[enemyIndex].m_iCurMoveIndex = 0;
			g_arrEnemy[enemyIndex].m_arrMovePos = info->m_Moves;

			enemyIndex++;
		}
	}
}

void EnemyUpdate(void)
{
	EnemyMove();
	EnemyAttack();
}

void EnemyMove(void)
{
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		// 비활성 enemy continue
		if (!g_arrEnemy[i].m_bIsActive)
			continue;

		stPos &nextPos = g_arrEnemy[i].m_arrMovePos[g_arrEnemy[i].m_iCurMoveIndex % g_arrEnemy[i].m_iMoveCount];

		g_arrEnemy[i].m_stPos.m_iY += nextPos.m_iY;
		g_arrEnemy[i].m_stPos.m_iX += nextPos.m_iX;

		g_arrEnemy[i].m_stPos.m_iY = max(g_arrEnemy[i].m_stPos.m_iY, 0);
		g_arrEnemy[i].m_stPos.m_iY = min(g_arrEnemy[i].m_stPos.m_iY, dfSCREEN_HEIGHT - 1);
		g_arrEnemy[i].m_stPos.m_iX = max(g_arrEnemy[i].m_stPos.m_iX, 0);
		g_arrEnemy[i].m_stPos.m_iX = min(g_arrEnemy[i].m_stPos.m_iX, dfSCREEN_WIDTH - 2);

		g_arrEnemy[i].m_iCurMoveIndex++;
	}
}

void EnemyAttack(void)
{
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		// 비활성 enemy continue
		if (!g_arrEnemy[i].m_bIsActive)
			continue;

		// 미사일 개수만큼 순회
		for (int j = 0; j < g_arrEnemy[i].m_iMissileCount; j++)
		{
			// 쿨타임이 지나지 않았으면 continue
			if (!TimerCalCoolTime(g_arrEnemy[i].m_arrMissilePrevTime[j], g_arrEnemy[i].m_MissileInfos[j].m_iCooltime))
				continue;

			// 미사일 생성
			stMissileInfo &info = g_arrEnemy[i].m_MissileInfos[j];
			MissileEnemyCreate(info, g_arrEnemy[i].m_stPos);
		}
	}
}

void EnemyRender(void)
{
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		// 비활성 enemy continue
		if (!g_arrEnemy[i].m_bIsActive)
			continue;

		ConsoleSpriteDraw(g_arrEnemy[i].m_stPos, g_arrEnemy[i].m_chShape);
	}
}
