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

	// 연결 구성요소 찾기
	int counter = 3; // 3부터 구성요소 번호
	for (int y = 0; y < m_iHeigth; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			// 미방문 공간 발견
			if (m_arrMap[x + m_iWidth * y] == 0)
			{
				Room room;
				room.counter;
				m_arrRooms.push_back(room);
				FindBoundarys(y, x, m_arrRooms.back().boundarys, counter);
				counter++;
			}
		}
	}

	SaveMap();
}

void CellularAutomata::SaveMap()
{
	for (int y = 0; y < m_iHeigth; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			if (m_arrMap[x + m_iWidth * y] == -1)
			{
				m_arrMap[x + m_iWidth * y] = 0;
			}

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
					if (m_arrMap[neighbourX + m_iWidth * neighbourY] == 1)
						wallCount += 1;
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

// BFS로 연결 구성요소 찾기 - 플러드 필 알고리즘
void CellularAutomata::FindBoundarys(int y, int x, std::vector<std::pair<int, int>> &component, int &counter)
{
	// 맵을 벗어나거나 y, x가 벽이면 리턴
	if (y < 0 || y >= m_iHeigth || x < 0 || x >= m_iWidth || m_arrMap[x + m_iWidth * y] == 1)
		return;

	std::vector<std::pair<int, int>> vec;
	vec.emplace_back(y, x);
	m_arrMap[x + m_iWidth * y] = counter; // 방문 표시

	while (!vec.empty())
	{
		int currentY = vec.back().first;
		int currentX = vec.back().second;
		vec.pop_back();

		if (GetSurrouningWallCount(currentY, currentX) != 0)
			component.emplace_back(currentY, currentX);

		// 상
		if (currentY > 0 && m_arrMap[(currentX)+m_iWidth * (currentY - 1)] == 0)
		{
			if (GetSurrouningWallCount(currentY - 1, currentX) == 0)
			{
				m_arrMap[currentX + m_iWidth * (currentY - 1)] = -1;
			}
			else
			{
				m_arrMap[currentX + m_iWidth * (currentY - 1)] = counter;
			}

			vec.emplace_back(currentY - 1, currentX);
		}

		// 하
		if (currentY < m_iHeigth - 1 && m_arrMap[(currentX)+m_iWidth * (currentY + 1)] == 0)
		{
			if (GetSurrouningWallCount(currentY + 1, currentX) == 0)
			{
				m_arrMap[currentX + m_iWidth * (currentY + 1)] = -1;
			}
			else
			{
				m_arrMap[currentX + m_iWidth * (currentY + 1)] = counter;
			}

			vec.emplace_back(currentY + 1, currentX);
		}

		// 좌
		if (currentX > 0 && m_arrMap[(currentX - 1) + m_iWidth * (currentY)] == 0)
		{
			if (GetSurrouningWallCount(currentY, currentX - 1) == 0)
			{
				m_arrMap[(currentX - 1) + m_iWidth * (currentY)] = -1;
			}
			else
			{
				m_arrMap[(currentX - 1) + m_iWidth * currentY] = counter;
			}

			vec.emplace_back(currentY, currentX - 1);
		}

		// 우
		if (currentX < m_iWidth - 1 && m_arrMap[(currentX + 1) + m_iWidth * (currentY)] == 0)
		{
			if (GetSurrouningWallCount(currentY, currentX + 1) == 0)
			{
				m_arrMap[(currentX + 1) + m_iWidth * (currentY)] = -1;
			}
			else
			{
				m_arrMap[(currentX + 1) + m_iWidth * currentY] = counter;
			}

			vec.emplace_back(currentY, currentX + 1);
		}
	}
}
