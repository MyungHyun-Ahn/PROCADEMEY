#include "pch.h"
#include "CellularAutomata.h"

CellularAutomata::CellularAutomata(int heigth, int width, int smoothCount, int randomFillPercent, time_t seed, bool useSeed)
	: m_iHeigth(heigth)
	, m_iWidth(width)
	, m_iSmoothCount(smoothCount)
	, m_iRandomFillPercent(randomFillPercent)
	, m_iSeed(seed)
	, m_bUseRandomSeed(useSeed)
{
	if (m_bUseRandomSeed)
		srand(m_iSeed);

	m_arrMap = new int[m_iHeigth * m_iWidth];
}

CellularAutomata::~CellularAutomata()
{
	delete m_arrMap;
}

void CellularAutomata::Generate()
{
	RandomFillMap();

	// 맵을 부드럽게 만드는 작업
	// 몇번 수행할 것인가 옵션
	for (int i = 0; i < m_iSmoothCount; i++)
	{
		SmoothMap();
	}

	SaveMap();
}

void CellularAutomata::SaveMap()
{
	for (int y = 0; y < m_iHeigth; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			g_Tile[y][x] = (TILE_TYPE)m_arrMap[x + m_iWidth * y];
		}
	}
}

void CellularAutomata::RandomFillMap()
{
	for (int y = 0; y < m_iHeigth; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			if (x == 0 || x == m_iWidth - 1 || y == 0 || y == m_iHeigth - 1)
			{
				m_arrMap[x + m_iWidth * y] = 1;
				continue;
			}
			
			m_arrMap[x + m_iWidth * y] = rand() % 100 < m_iRandomFillPercent ? 1 : 0;
		}
	}
}

void CellularAutomata::SmoothMap()
{
	for (int y = 0; y < m_iHeigth; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			int neighbourWallCount = GetSurrouningWallCount(y, x);

			if (neighbourWallCount > 4)
				m_arrMap[x + m_iWidth * y] = 1;
			else if (neighbourWallCount < 4)
				m_arrMap[x + m_iWidth * y] = 0;
		}
	}
}

int CellularAutomata::GetSurrouningWallCount(int gridY, int gridX)
{
	int wallCount = 0;

	for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY++)
	{
		for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX++)
		{
			// 경계 체크
			if (neighbourY >= 0 && neighbourY < m_iHeigth && neighbourX >= 0 && neighbourX < m_iWidth)
			{
				if (neighbourY != gridY || neighbourX != gridX)
				{
					wallCount += m_arrMap[neighbourX + m_iWidth * neighbourY];
				}
			}
			else
			{
				wallCount++;
			}
		}
	}

	return wallCount;
}
