#include "pch.h"
#include "Enemy.h"
#include "Console.h"
#include "Missile.h"
#include "TimeManager.h"
#include "StageInfo.h"
#include "SceneManager.h"

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
			g_arrEnemy[enemyIndex].m_iSpeed = info->m_iSpeed;					// Speed

			// 미사일 정보 세팅
			g_arrEnemy[enemyIndex].m_iMissileCount = info->m_iMissileCount;		// 미사일 종류
			g_arrEnemy[enemyIndex].m_MissileInfos = info->m_MissileInfos;		// 가져다 쓸 것 - 얕은 복사

			g_arrEnemy[enemyIndex].m_arrMissilePrevTime = (unsigned int *)malloc(sizeof(unsigned int) * info->m_iMissileCount);
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
			g_arrEnemy[enemyIndex].m_dCurMoveIndex = 0.;
			g_arrEnemy[enemyIndex].m_arrMovePos = info->m_Moves;

			enemyIndex++;
		}
	}

	// 모든 적을 초기화하고 스테이지 클리어 정보를 false로 초기화
	g_bIsStageClear = false;
}

void EnemyRelease(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	if (g_arrEnemy != nullptr)
	{
		for (int i = 0; i < g_iEnemyCount; i++)
		{
			if (g_arrEnemy[i].m_arrMissilePrevTime != nullptr)
			{
				free(g_arrEnemy[i].m_arrMissilePrevTime);
				g_arrEnemy[i].m_arrMissilePrevTime = nullptr;
			}
		}

		free(g_arrEnemy);
		g_arrEnemy = nullptr;
	}
}

void EnemyUpdate(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	EnemyMove();
	EnemyAttack();
	EnemyClearCheck();
}

void EnemyMove(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		// 비활성 enemy continue
		if (!g_arrEnemy[i].m_bIsActive)
			continue;

		g_arrEnemy[i].m_dCurMoveIndex += g_arrEnemy[i].m_iSpeed * dfFIXED_DELTATIME;
		// m_dDirIndex를 int 로 캐스팅한게 m_iDirIndex 보다 커지면
		// 다음 칸으로 이동 가능한 것
		if ((int)g_arrEnemy[i].m_dCurMoveIndex < g_arrEnemy[i].m_iCurMoveIndex)
			continue;

		g_arrEnemy[i].m_iCurMoveIndex++;

		int index = (g_arrEnemy[i].m_iCurMoveIndex - 1) % g_arrEnemy[i].m_iMoveCount;

		stPos &nextPos = g_arrEnemy[i].m_arrMovePos[index];

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
	PROFILE_BEGIN(__WFUNC__, 0);
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

void EnemyGetDamage(int enemyIndex, int damage)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	g_arrEnemy[enemyIndex].m_iCurHp -= damage;

	if (g_arrEnemy[enemyIndex].m_iCurHp <= 0)
	{
		// enemy 비활성화
		g_arrEnemy[enemyIndex].m_bIsActive = false;
	}
}

void EnemyClearCheck(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	// 모든 적이 죽었는가를 판단.
	bool allEnemyIsDead = true;

	for (int i = 0; i < g_iEnemyCount; i++)
	{
		// 1명이라도 살아있는 적이 있으면 break;
		if (g_arrEnemy[i].m_bIsActive)
		{
			allEnemyIsDead = false;
			break;
		}
	}

	g_bIsStageClear = allEnemyIsDead;
}

void EnemyRender(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		// 비활성 enemy continue
		if (!g_arrEnemy[i].m_bIsActive)
			continue;

		ConsoleSpriteDraw(g_arrEnemy[i].m_stPos, g_arrEnemy[i].m_chShape);
	}
}
