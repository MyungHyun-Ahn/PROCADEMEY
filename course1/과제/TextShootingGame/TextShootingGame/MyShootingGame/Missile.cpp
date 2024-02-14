#include "pch.h"
#include "Missile.h"
#include "Player.h"
#include "Enemy.h"
#include "Console.h"

void MissileEnemyCreate(const stMissileInfo &info, const stPos &curPos)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		// 활성화된 미사일 패스
		if (g_arrMissile[i].m_bIsActive)
			continue;

		// 기본 정보
		g_arrMissile[i].m_bIsActive = true;
		g_arrMissile[i].m_bIsEnemy = true;
		g_arrMissile[i].m_chShape = info.m_chShape;
		g_arrMissile[i].m_iDamage = info.m_iDamage;

		// 몇 프레임 존재할 수 있는지
		g_arrMissile[i].m_iCurLife = 0;
		g_arrMissile[i].m_iMaxLife = info.m_iLifetime;

		// 시작 좌표 세팅
		g_arrMissile[i].m_stPos.m_iY = curPos.m_iY + info.m_iStartPos.m_iY;
		g_arrMissile[i].m_stPos.m_iX = curPos.m_iX + info.m_iStartPos.m_iX;

		// 진행 방향 세팅
		g_arrMissile[i].m_iDirIndex = 0;
		g_arrMissile[i].m_iDirSize = info.m_iMissileMaxMoveIndex;
		g_arrMissile[i].m_arrDir = info.m_MissileMoves;

		// 하나 만들었으면 빠져나오기
		break;
	}
}

void MissileRelease(void)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		g_arrMissile[i].m_bIsActive = false;
	}
}

void MissilePlayerDefualtCreate(const stPos &curDir)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		// 활성화된 미사일 패스
		if (g_arrMissile[i].m_bIsActive)
			continue;

		// 기본 정보
		g_arrMissile[i].m_bIsActive = true;
		g_arrMissile[i].m_bIsEnemy = false;
		g_arrMissile[i].m_chShape = dfPLAYER_MISSILE_SHAPE;
		g_arrMissile[i].m_iDamage = g_stPlayer.m_iDamage;

		// 몇 프레임 존재할 수 있는지
		g_arrMissile[i].m_iCurLife = 0;
		g_arrMissile[i].m_iMaxLife = INT32_MAX;

		// 진행 방향 세팅
		g_arrMissile[i].m_iDirIndex = 0;
		g_arrMissile[i].m_iDirSize = 1;
		g_arrMissile[i].m_DirBak = curDir;
		g_arrMissile[i].m_arrDir = &g_arrMissile[i].m_DirBak;

		// 시작 좌표 세팅
		g_arrMissile[i].m_stPos.m_iY = g_stPlayer.m_stPos.m_iY + curDir.m_iY;
		g_arrMissile[i].m_stPos.m_iX = g_stPlayer.m_stPos.m_iX + curDir.m_iX;
		
		break;
	}
}

void MissileUpdate(void)
{
	MissileMove();
	MissileCollision();
}

void MissileMove(void)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		// 비활성화 미사일 패스
		if (!g_arrMissile[i].m_bIsActive)
			continue;

		stPos &nextDir = g_arrMissile[i].m_arrDir[g_arrMissile[i].m_iDirIndex % g_arrMissile[i].m_iDirSize];
		g_arrMissile[i].m_stPos.m_iY += nextDir.m_iY;
		g_arrMissile[i].m_stPos.m_iX += nextDir.m_iX;

		g_arrMissile[i].m_iCurLife++;

		// 생명 프레임 초과
		if (g_arrMissile[i].m_iCurLife > g_arrMissile[i].m_iMaxLife)
		{
			g_arrMissile[i].m_bIsActive = false;
		}

		// 화면 밖으로 벗어난 경우 비활성화
		if (g_arrMissile[i].m_stPos.m_iY > dfSCREEN_HEIGHT || g_arrMissile[i].m_stPos.m_iX > dfSCREEN_WIDTH - 1)
		{
			g_arrMissile[i].m_bIsActive = false;
		}

		if (g_arrMissile[i].m_stPos.m_iY < 0 || g_arrMissile[i].m_stPos.m_iX < 0)
		{
			g_arrMissile[i].m_bIsActive = false;
		}
	}
}

void MissileCollision(void)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		// 비활성화 미사일 패스
		if (!g_arrMissile[i].m_bIsActive)
			continue;

		stPos &mPos = g_arrMissile[i].m_stPos;

		// Player에 충돌했는지 판단 - Player는 1개이므로 먼저 판단
		// 적이 쏜 경우에만
		if (g_arrMissile[i].m_bIsEnemy)
		{
			// 좌표가 겹친 경우에 충돌
			if (!(mPos.m_iY == g_stPlayer.m_stPos.m_iY && mPos.m_iX == g_stPlayer.m_stPos.m_iX))
				continue;

			// 충돌처리 - 데미지
			PlayerGetDamage(g_arrMissile[i].m_iDamage);

			// 미사일 비활성화 - 미사일은 관통하지 않는다.
			// 한번 충돌하고 비활성화
			g_arrMissile[i].m_bIsActive = false;
			break;
		}
		else // Player가 쏜 것
		{
			for (int j = 0; j < g_iEnemyCount; j++)
			{
				// enemy가 이미 죽은 경우
				if (!g_arrEnemy[j].m_bIsActive)
					continue;

				stPos &ePos = g_arrEnemy[j].m_stPos;

				// 좌표가 겹친 경우에 충돌
				if (!(mPos.m_iY == ePos.m_iY && mPos.m_iX == ePos.m_iX))
					continue;

				// 충돌처리 - 데미지
				EnemyGetDamage(j, g_arrMissile[i].m_iDamage);

				// 미사일 비활성화 - 미사일은 관통하지 않는다.
				// 한번 충돌하고 비활성화
				g_arrMissile[i].m_bIsActive = false;
				break;
			}
		}
	}
}

void MissileRender(void)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		// 비활성화 미사일 패스
		if (!g_arrMissile[i].m_bIsActive)
			continue;

		ConsoleSpriteDraw(g_arrMissile[i].m_stPos, g_arrMissile[i].m_chShape);
	}
}
