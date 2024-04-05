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

		node->pqPop = true; // CloseList üũ

		// �������� YELLOW Ÿ��, �� ������� �������� �ƴ� ���
		if (!((node->x == m_startNode->x && node->y == m_startNode->y) ||
			(node->x == m_destNode->x && node->y == m_destNode->y)))
		{
			g_Tile[node->y][node->x] = TILE_TYPE::YELLOW;
		}

		// �������� ã�� ��
		if (node->x == m_destNode->x && node->y == m_destNode->y)
		{
			g_Tile[node->y][node->x] = TILE_TYPE::RED;
			InvalidateRect(hWnd, NULL, false);
			UpdateWindow(hWnd);
			break;
		}

		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);

		// ��� Ž���ϰ� ��� �����
		// 8����
		for (int i = 0; i < 4; i++)
		{
			if ((node->dir & (uint8_t)g_maskTable[i]) != (uint8_t)g_maskTable[i])
				continue;

			// �ش� �������� �����ϸ� üũ
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
					// �ڳ��� ��Ȳ, ��带 ����� ���� ����
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


					// ����ġ�� ���ο� ���� �� �۴ٸ�
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
