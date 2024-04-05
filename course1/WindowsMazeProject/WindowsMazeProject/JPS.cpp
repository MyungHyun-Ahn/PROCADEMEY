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
		// 8방향
		for (int i = 0; i < 4; i++)
		{
			if ((node->dir & (uint8_t)g_maskTable[i]) != (uint8_t)g_maskTable[i])
				continue;

			// 해당 방향으로 전진하며 체크
			int curY = node->y + g_Dir[i][0];
			int curX = node->x + g_Dir[i][1];

			while (true)
			{
				if (!OutMap(curY, curX))
					break;

				if (curY == destY && curX == destX)
				{
					Node *destNode = new Node(curY, curX, 0);
					destNode->parent = node;
					g_arrNodes[destNode->y][destNode->x] = destNode;
					m_Pq.push(*destNode);
					break;
				}

				Node *newNode = CheckCorner((DIR_MASK)g_maskTable[i], curY, curX);

				if (newNode != nullptr)
				{
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

		for (int i = 4; i < 8; i++)
		{

		}
	}
}

void JPS::PrintRoute(HWND hWnd)
{
}
