#include "pch.h"
#include "Missile.h"
#include "Console.h"

stMissile g_arrMissile[dfMAX_MISSILE_COUNT];

void MissileInit()
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		g_arrMissile[i] = stMissile{ false };
	}
}

void MissileCreate(bool isEnemy, int posY, int posX, const stPos &&dir)
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		// 이미 활성화된 미사일 건너뛰기
		if (g_arrMissile[i].m_bIsActive)
			continue;

		// 미사일 세팅
		g_arrMissile[i].m_bIsActive = true;
		g_arrMissile[i].m_bIsEnemy = true;
		g_arrMissile[i].m_stPos = stPos{ posY, posX };
		g_arrMissile[i].m_stDir = dir;

		// 방향에 따른 미사일 Shape 판단
		g_arrMissile[i].m_chShape = MissileGetShape(g_arrMissile[i].m_stDir);
		break;
	}
}

wchar_t MissileGetShape(stPos &dir)
{
	//
	//	   ◤  ▲  ◥
	//	  ◀  #  ▶
	//	   ◣  ▼  ◢
	//

	if (dir.m_iY == 0)
	{
		if (dir.m_iX == -1)
		{
			// 왼
			return L'<';
		}

		if (dir.m_iX == 1)
		{
			// 오
			return L'>';
		}
	}

	if (dir.m_iX == 0)
	{
		if (dir.m_iY == -1)
		{
			// 위
			return L'^';
		}

		if (dir.m_iY == 1)
		{
			// 아래
			return L'v';
		}
	}

	if (dir.m_iY == 1)
	{
		if (dir.m_iX == -1)
		{
			// 위왼
			return L'v';
		}

		if (dir.m_iX == 1)
		{
			// 위오
			return L'v';
		}
	}
	else
	{
		if (dir.m_iX == -1)
		{
			// 아왼
			return L'^';
		}

		if (dir.m_iX == 1)
		{
			// 아오
			return L'^';
		}
	}

	// 잘못된 경우
	return L' ';
}

void MissileUpdate()
{
	MissileMove();
}

void MissileMove()
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		if (!g_arrMissile[i].m_bIsActive)
			continue;

		stPos dir = g_arrMissile[i].m_stDir;
		g_arrMissile[i].m_stPos.m_iY += dir.m_iY;
		g_arrMissile[i].m_stPos.m_iX += dir.m_iX;

		// 맵을 벗어나면 삭제
		if (g_arrMissile[i].m_stPos.m_iY > dfSCREEN_HEIGHT - 2 || g_arrMissile[i].m_stPos.m_iX > dfSCREEN_WIDTH - 2)
			g_arrMissile[i].m_bIsActive = false;
	}
}

void MissileDraw()
{
	for (int i = 0; i < dfMAX_MISSILE_COUNT; i++)
	{
		if (!g_arrMissile[i].m_bIsActive)
			continue;

		ConsoleSpriteDraw(g_arrMissile[i].m_stPos, g_arrMissile[i].m_chShape);
	}
}

void MissileCollison()
{
}
