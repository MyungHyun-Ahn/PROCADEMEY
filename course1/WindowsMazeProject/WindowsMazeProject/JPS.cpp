#include "pch.h"
#include "Node.h"
#include "JPS.h"

DIR_MASK g_maskTable[8] = {
	DIR_MASK::U,
	DIR_MASK::D,
	DIR_MASK::L,
	DIR_MASK::R,
	DIR_MASK::UL,
	DIR_MASK::UR,
	DIR_MASK::DL,
	DIR_MASK::DR,
};

void JPS::Search(HWND hWnd, HDC hdc)
{
	while (!m_Pq.empty())
	{
		Node popNode = m_Pq.top();
		m_Pq.pop();

		Node *node = g_arrNodes[popNode.y][popNode.x];
		if (node == NULL)
			throw;

		node->pqPop = true; // CloseList 체크

		// 꺼냈으면 YELLOW 타일, 단 출발지와 목적지가 아닐 경우
		if (!((node->x == m_startNode->x && node->y == m_startNode->y) ||
			(node->x == m_destNode->x && node->y == m_destNode->y)))
		{
			g_Tile[node->y][node->x] = TILE_TYPE::YELLOW;
		}

		// 목적지를 찾은 것
		if (node->x == m_destNode->x && node->y == m_destNode->y)
		{
			g_Tile[node->y][node->x] = TILE_TYPE::RED;
			InvalidateRect(hWnd, NULL, false);
			UpdateWindow(hWnd);
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);

		// 경로 탐색하고 노드 만들기
		// 4방향 직진하는 경우
		for (int i = 0; i < 4; i++)
		{
			// 이쪽 방향으로 탐색을 해야하는지
			if ((node->dir & (uint8_t)g_maskTable[i]) != (uint8_t)g_maskTable[i])
				continue;
			
			// 해당 방향으로 전진하며 체크
			int curY = node->y + g_Dir[i][0];
			int curX = node->x + g_Dir[i][1];

			while (true)
			{
				if (!OutMap(curY + g_Dir[i][0], curX + g_Dir[i][1]))
					break;

				if (!CheckObstacle(curY, curX))
					break;

				if (curY == destY && curX == destX)
				{
					Node *destNode = new Node(curY, curX, 0);
					destNode->parent = node;
					g_arrNodes[destNode->y][destNode->x] = destNode;
					m_Pq.push(*destNode);
					break;
				}

				uint8_t dir = CheckCorner(g_maskTable[i], curY, curX);

				if (dir != 0)
				{
					Node *newNode = new Node(curY, curX, dir);

					// 코너인 상황, 노드를 만들고 루프 종료
					newNode->parent = node;
					
					if (m_bModeG)
						newNode->G = node->G + CalManhattanG(*newNode);
					else
						newNode->G = node->G + CalEuclideanG(*newNode);
					if (m_bModeH)
						newNode->H = CalManhattanH(*node);
					else
						newNode->H = CalEuclideanH(*node);

					newNode->F = newNode->G + newNode->H;


					// 가중치가 새로운 것이 더 작다면
					if (g_arrNodes[newNode->y][newNode->x] != NULL)
					{
						if (newNode->F < g_arrNodes[newNode->y][newNode->x]->F)
						{
							delete g_arrNodes[newNode->y][newNode->x];
							g_arrNodes[newNode->y][newNode->x] = newNode;
							m_Pq.push(*newNode);
						}
						else
						{
							delete newNode;
						}
					}
					else
					{
						g_arrNodes[newNode->y][newNode->x] = newNode;
						m_Pq.push(*newNode);
						g_Tile[newNode->y][newNode->x] = TILE_TYPE::BLUE;
						InvalidateRect(hWnd, NULL, false);
						UpdateWindow(hWnd);
					}
					break;
				}

				curY = curY + g_Dir[i][0];
				curX = curX + g_Dir[i][1];
			}
		}

		// 대각선 방향
		for (int i = 4; i < 8; i++)
		{
			if ((node->dir & (uint8_t)g_maskTable[i]) != (uint8_t)g_maskTable[i])
				continue;

			// 해당 방향으로 전진하며 체크
			int curY = node->y + g_Dir[i][0];
			int curX = node->x + g_Dir[i][1];

			while (true)
			{
				if (!OutMap(curY + g_Dir[i][0], curX + g_Dir[i][1]))
					break;

				if (!CheckObstacle(curY, curX))
					break;

				if (curY == destY && curX == destX)
				{
					Node *destNode = new Node(curY, curX, 0);
					destNode->parent = node;
					g_arrNodes[destNode->y][destNode->x] = destNode;
					m_Pq.push(*destNode);
					break;
				}

				// 직선 방향 벽 검사
				uint8_t dir = CheckFront(g_maskTable[i], curY, curX);
				// 코너 검사
				dir |= CheckCorner(g_maskTable[i], curY, curX);

				if (dir != 0)
				{
					dir |= (uint8_t)g_maskTable[i];

					// 코너인 상황, 노드를 만들고 루프 종료
					Node *newNode = new Node(curY, curX, dir);

					newNode->parent = node;

					if (m_bModeG)
						newNode->G = node->G + CalManhattanG(*newNode);
					else
						newNode->G = node->G + CalEuclideanG(*newNode);
					if (m_bModeH)
						newNode->H = CalManhattanH(*node);
					else
						newNode->H = CalEuclideanH(*node);

					newNode->F = newNode->G + newNode->H;


					// 가중치가 새로운 것이 더 작다면
					if (g_arrNodes[newNode->y][newNode->x] != NULL)
					{
						if (newNode->F < g_arrNodes[newNode->y][newNode->x]->F)
						{
							delete g_arrNodes[newNode->y][newNode->x];
							g_arrNodes[newNode->y][newNode->x] = newNode;
							m_Pq.push(*newNode);
						}
						else
						{
							delete newNode;
						}
					}
					else
					{
						g_arrNodes[newNode->y][newNode->x] = newNode;
						m_Pq.push(*newNode);
						g_Tile[newNode->y][newNode->x] = TILE_TYPE::BLUE;
						InvalidateRect(hWnd, NULL, false);
						UpdateWindow(hWnd);
					}
					break;
				}

				curY = curY + g_Dir[i][0];
				curX = curX + g_Dir[i][1];
			}
		}
	}
}

void JPS::PrintRoute(HWND hWnd)
{
}

uint8_t JPS::CheckFront(DIR_MASK dirType, int y, int x)
{
	uint8_t dir = 0;

	switch (dirType)
	{
	case DIR_MASK::UR:
	{
		if (FrontSearch(DIR_MASK::U, y, x))
			dir |= (uint8_t)DIR_MASK::U;

		if (FrontSearch(DIR_MASK::R, y, x))
			dir |= (uint8_t)DIR_MASK::R;
	}
		
		break;
	case DIR_MASK::UL:
	{
		if (FrontSearch(DIR_MASK::U, y, x))
			dir |= (uint8_t)DIR_MASK::U;

		if (FrontSearch(DIR_MASK::L, y, x))
			dir = (uint8_t)DIR_MASK::L;
	}
		break;
	case DIR_MASK::DR:
	{
		if (FrontSearch(DIR_MASK::D, y, x))
			dir |= (uint8_t)DIR_MASK::D;

		if (FrontSearch(DIR_MASK::R, y, x))
			dir |= (uint8_t)DIR_MASK::R;
	}
		break;
	case DIR_MASK::DL:
	{
		if (FrontSearch(DIR_MASK::D, y, x))
			dir |= (uint8_t)DIR_MASK::D;

		if (FrontSearch(DIR_MASK::L, y, x))
			dir = (uint8_t)DIR_MASK::L;
	}
		break;
	}

	return dir;
}

bool JPS::FrontSearch(DIR_MASK dirType, int y, int x)
{
	switch (dirType)
	{
	case DIR_MASK::U:
	{
		int curY = y;
		int curX = x;
		while (true)
		{
			if (!OutMap(curY + g_Dir[0][0], curX + g_Dir[0][1]))
				break;

			if (!CheckObstacle(curY, curX))
				break;

			if (curY == destY && curX == destX)
				return true;

			if (CheckCorner(DIR_MASK::U, curY, curX) != 0)
				return true;

			curY += g_Dir[0][0];
			curX += g_Dir[0][1];
		}
	}
		break;
	case DIR_MASK::D:
	{
		int curY = y;
		int curX = x;
		while (true)
		{
			if (!OutMap(curY + g_Dir[1][0], curX + g_Dir[1][1]))
				break;

			if (!CheckObstacle(curY, curX))
				break;

			if (curY == destY && curX == destX)
				return true;

			if (CheckCorner(DIR_MASK::D, curY, curX) != 0)
				return true;

			curY += g_Dir[1][0];
			curX += g_Dir[1][1];
		}
	}
		break;
	case DIR_MASK::L:
	{
		int curY = y;
		int curX = x;
		while (true)
		{
			if (!OutMap(curY + g_Dir[2][0], curX + g_Dir[2][1]))
				break;

			if (!CheckObstacle(curY, curX))
				break;

			if (curY == destY && curX == destX)
				return true;

			if (CheckCorner(DIR_MASK::L, curY, curX) != 0)
				return true;

			curY += g_Dir[2][0];
			curX += g_Dir[2][1];
		}
	}
		break;
	case DIR_MASK::R:
	{
		int curY = y;
		int curX = x;
		while (true)
		{
			if (!OutMap(curY + g_Dir[3][0], curX + g_Dir[3][1]))
				break;

			if (!CheckObstacle(curY, curX))
				break;

			if (curY == destY && curX == destX)
				return true;

			if (CheckCorner(DIR_MASK::R, curY, curX) != 0)
				return true;

			curY += g_Dir[3][0];
			curX += g_Dir[3][1];
		}
	}
		break;
	}
	return false;
}
