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
				room.counter = counter;;
				m_arrRooms.push_back(room);
				FindBoundarys(y, x, counter);
				counter++;
			}
		}
	}

	CalDistances();
	Prim();
	Merge();
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
void CellularAutomata::FindBoundarys(int y, int x, int &counter)
{
	// 맵을 벗어나거나 y, x가 벽이면 리턴
	if (y < 0 || y >= m_iHeigth || x < 0 || x >= m_iWidth || m_arrMap[x + m_iWidth * y] == 1)
		return;

	std::vector<Pos> vec;
	vec.emplace_back(Pos{ y, x });
	m_arrMap[x + m_iWidth * y] = counter; // 방문 표시

	while (!vec.empty())
	{
		Pos curruntPos = vec.back();
		vec.pop_back();

		int currentY = curruntPos.y;
		int currentX = curruntPos.x;

		if (GetSurrouningWallCount(currentY, currentX) != 0)
			m_arrRooms.back().boundarys.emplace_back(Pos{ currentY, currentX });

		// 상
		if (currentY > 0 && m_arrMap[(currentX)+m_iWidth * (currentY - 1)] == 0)
		{
			if (GetSurrouningWallCount(currentY - 1, currentX) < 2)
			{
				m_arrMap[currentX + m_iWidth * (currentY - 1)] = -1;
			}
			else
			{
				m_arrMap[currentX + m_iWidth * (currentY - 1)] = counter;
			}

			vec.emplace_back(Pos{ currentY - 1, currentX });
		}

		// 하
		if (currentY < m_iHeigth - 1 && m_arrMap[(currentX)+m_iWidth * (currentY + 1)] == 0)
		{
			if (GetSurrouningWallCount(currentY + 1, currentX) < 2)
			{
				m_arrMap[currentX + m_iWidth * (currentY + 1)] = -1;
			}
			else
			{
				m_arrMap[currentX + m_iWidth * (currentY + 1)] = counter;
			}

			vec.emplace_back(Pos{ currentY + 1, currentX });
		}

		// 좌
		if (currentX > 0 && m_arrMap[(currentX - 1) + m_iWidth * (currentY)] == 0)
		{
			if (GetSurrouningWallCount(currentY, currentX - 1) < 2)
			{
				m_arrMap[(currentX - 1) + m_iWidth * (currentY)] = -1;
			}
			else
			{
				m_arrMap[(currentX - 1) + m_iWidth * currentY] = counter;
			}

			vec.emplace_back(Pos{ currentY, currentX - 1 });
		}

		// 우
		if (currentX < m_iWidth - 1 && m_arrMap[(currentX + 1) + m_iWidth * (currentY)] == 0)
		{
			if (GetSurrouningWallCount(currentY, currentX + 1) < 2)
			{
				m_arrMap[(currentX + 1) + m_iWidth * (currentY)] = -1;
			}
			else
			{
				m_arrMap[(currentX + 1) + m_iWidth * currentY] = counter;
			}

			vec.emplace_back(Pos{ currentY, currentX + 1 });
		}
	}
}

void CellularAutomata::CalDistances()
{
	for (int i = 0; i < m_arrRooms.size(); i++)
	{
		for (int j = 0; j < m_arrRooms.size(); j++)
		{
			// 자기 자신과 중복 케이스 pass
			if (i == j)
				continue;

			auto it1 = m_arrRooms[i].distances.find(j);
			if (it1 == m_arrRooms[i].distances.end())
			{
				m_arrRooms[i].distances[j] = DBL_MAX;
			}

			auto it2 = m_arrRooms[i].positions.find(j);
			if (it2 == m_arrRooms[i].positions.end())
			{
				m_arrRooms[i].positions[j] = std::make_pair<Pos, Pos>(Pos{ 0, 0 }, Pos{ 0, 0 });
			}

			// 모든 경계선 기준으로 distance 계산
			std::vector<Pos> &aBoundarys = m_arrRooms[i].boundarys;
			std::vector<Pos> &bBoundarys = m_arrRooms[j].boundarys;


			
			for (int a = 0; a < aBoundarys.size(); a++)
			{
				Pos &aPos = aBoundarys[a];
				for (int b = 0; b < bBoundarys.size(); b++)
				{
					Pos &bPos = bBoundarys[b];

					double distance = CalEuclidean(aPos, bPos);

					if (distance < m_arrRooms[i].distances[j])
					{
						m_arrRooms[i].distances[j] = distance;
						m_arrRooms[i].positions[j] = std::make_pair<Pos, Pos>(Pos{ aPos.y, aPos.x }, Pos{ bPos.y, bPos.x });
					}
				}
			}
		}
	}

}
/*
Prim 알고리즘 구현 방법
1. 임의의 시작점 하나 선택
2. 이 시작점과 연결된 정점들의 거리 업데이트
3. 선택된 정점들과 연결되어 있는 간선 중, 가장 짧은 길이의 간선을 선택해서 연결
4. 가장 짧은 간선으로 연결되어 있는 정점을 선택하고, 정점들의 거리를 업데이트
5. 3 ~ 4번의 과정을 전체 노드의 수 - 1 번 만큼 반복
*/
void CellularAutomata::Prim()
{
	// distance , to, from
	std::priority_queue<std::pair<double, std::pair<int, int>>> pq;
	m_arrRooms[0].isVisited = true; // 방문 처리
	for (auto distance : m_arrRooms[0].distances)
	{
		int next = distance.first;
		double dist = distance.second;
		pq.push(std::make_pair(-dist, std::make_pair(0, next)));
	}

	while (!pq.empty())
	{
		std::pair<double, std::pair<int, int>> p = pq.top();
		double dist = -p.first;
		int to = p.second.first;
		int from = p.second.second;

		pq.pop();

		if (!m_arrRooms[from].isVisited)
		{
			// 연결처리
			m_arrRooms[to].toFroms.emplace_back(from);

			m_arrRooms[from].isVisited = true;

			for (auto distance : m_arrRooms[from].distances)
			{
				int nNext = distance.first;
				double nDistance = distance.second;

				if (!m_arrRooms[nNext].isVisited)
				{
					pq.push(std::make_pair(-nDistance, std::make_pair(from, nNext)));
				}
			}
		}
	}
}

void CellularAutomata::Merge()
{
	for (auto room : m_arrRooms)
	{
		for (auto from : room.toFroms)
		{
			Pos &startPos = room.positions[from].first;
			Pos &destPos = room.positions[from].second;

			Bresenham(startPos.y, startPos.x, destPos.y, destPos.x);
		}
	}
}

void CellularAutomata::Bresenham(int startY, int startX, int destY, int destX)
{
	// start와 dest 반전 처리
	bool reversed = false;
	if (abs(destX - startX) < abs(destY - startY)) {
		std::swap(startX, startY);
		std::swap(destX, destY);
		reversed = true;
	}

	// 시작점이 목적지보다 오른쪽에 있는 경우 교환
	if (startX > destX) {
		std::swap(startX, destX);
		std::swap(startY, destY);
	}

	int dx = destX - startX;
	int dy = abs(destY - startY);
	int err = dx / 2;
	int ystep = (startY < destY) ? 1 : -1;

	for (int x = startX, y = startY; x <= destX; x++) {
		if (reversed) {
			std::cout << "Cellular automata merge : " << x << " " << y << std::endl;
			m_arrMap[y + m_iWidth * x] = 0;
		}
		else {
			std::cout << "Cellular automata merge: " << y << " " << x << std::endl;
			m_arrMap[x + m_iWidth * y] = 0;
		}

		err -= dy;
		if (err < 0) {
			y += ystep;
			err += dx;
		}
	}
}
