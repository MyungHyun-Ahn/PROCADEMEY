#include "pch.h"
#include "Enemy.h"
#include "Console.h"
#include "Missile.h"

int g_iEnemyCount;
stEnemy *g_arrEnemy = nullptr;

void EnemyInit(int enemyCount)
{
	g_iEnemyCount = enemyCount;

	g_arrEnemy = (stEnemy *)malloc(sizeof(stEnemy) * g_iEnemyCount);

	// TODO: ������ �о� enemy ���� ����

	// ������ �׳� �׽�Ʈ ��
	int startPos = 20;
	int endPos = dfSCREEN_WIDTH - 20;
	int xGap = (endPos - startPos) / (g_iEnemyCount / 4); // 4�� ��ġ
	int yGap = 3;


	int curEnemyCount = 0;
	int curXLine = 0;
	int curYLine = 1;
	while (curEnemyCount < g_iEnemyCount)
	{
		if (g_arrEnemy == nullptr)
			return;

		g_arrEnemy[curEnemyCount].m_stPos.m_iY = yGap * curYLine;
		g_arrEnemy[curEnemyCount].m_stPos.m_iX = startPos + xGap * curXLine;
		g_arrEnemy[curEnemyCount].m_bIsActive = true;

		g_arrEnemy[curEnemyCount].m_chShape = L'E';

		// �� �� 4ĭ �̵�
		g_arrEnemy[curEnemyCount].m_iPosCount = 8;
		g_arrEnemy[curEnemyCount].m_iCurPos = 0;
		g_arrEnemy[curEnemyCount].m_ArrPos = (stPos *)malloc(sizeof(stPos) * g_arrEnemy[curEnemyCount].m_iPosCount);
		if (g_arrEnemy[curEnemyCount].m_ArrPos == nullptr)
			return;

		// �̵� ��� �ʱ�ȭ
		g_arrEnemy[curEnemyCount].m_ArrPos[0] = stPos{ 0, 1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[1] = stPos{ 0, 1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[2] = stPos{ 0, 1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[3] = stPos{ 0, 1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[4] = stPos{ 0, -1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[5] = stPos{ 0, -1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[6] = stPos{ 0, -1 };
		g_arrEnemy[curEnemyCount].m_ArrPos[7] = stPos{ 0, -1 };

		curEnemyCount++;
		curXLine++;

		if (curEnemyCount % (g_iEnemyCount / 4) == 0)
		{
			curYLine++;
			curXLine = 0;
		}
	}
}

void EnemyReset(void)
{
	if (g_arrEnemy != nullptr)
	{
		for (int i = 0; i < g_iEnemyCount; i++)
		{
			if (g_arrEnemy[i].m_ArrPos != nullptr)
				free(g_arrEnemy[i].m_ArrPos);

			g_arrEnemy[i].m_ArrPos = nullptr;
		}

		free(g_arrEnemy);
		g_arrEnemy = nullptr;
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
		stPos nextPos = g_arrEnemy[i].m_ArrPos[g_arrEnemy[i].m_iCurPos++];
		g_arrEnemy[i].m_iCurPos %= g_arrEnemy[i].m_iPosCount;

		g_arrEnemy[i].m_stPos.m_iY += nextPos.m_iY;
		g_arrEnemy[i].m_stPos.m_iX += nextPos.m_iX;
	}
}

void EnemyAttack(void)
{
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		if (!g_arrEnemy[i].m_bIsActive)
			continue;

		stPos nextDir{ 1,0 };
		MissileCreate(true, g_arrEnemy[i].m_stPos.m_iY + nextDir.m_iY, g_arrEnemy[i].m_stPos.m_iX + nextDir.m_iX, std::move(nextDir));
	}
}

void EnemyDraw(void)
{
	for (int i = 0; i < g_iEnemyCount; i++)
	{
		stEnemy curEnemy = g_arrEnemy[i];
		if (curEnemy.m_bIsActive)
			ConsoleSpriteDraw(curEnemy.m_stPos, curEnemy.m_chShape);
	}
}
