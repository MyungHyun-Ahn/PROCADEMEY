#include "pch.h"
#include "Node.h"
#include "AStar.h"

void AStar::Search(HWND hWnd, HDC hdc)
{
	while (!m_Pq.empty())
	{
		Node popNode = m_Pq.top();
		m_Pq.pop();

		Node *node = g_arrNodes[popNode.y][popNode.x];
		if (node == NULL)
			throw;

		node->pqPop = true;
		curNode = node;
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

		//Sleep(1000);
		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);

		for (int i = 0; i < 8; i++)
		{
			Node *nextNode = new Node;

			// ���� ��ǥ
			nextNode->y = node->y + g_Dir[i][0];
			nextNode->x = node->x + g_Dir[i][1];

			if (!CanGo(nextNode->y, nextNode->x))
			{
				// �� ���� ����
				delete nextNode;
				continue;
			}

			if (i < 4)
				nextNode->G = node->G + 1;
			else
			{
				if (m_bModeG)
				{
					nextNode->G = node->G + Manhattan;
				}
				else
				{
					nextNode->G = node->G + Euclidean;
				}
			}

			if (m_bModeH)
				nextNode->H = CalManhattanDistance(*nextNode);
			else
				nextNode->H = CalEuclideanDistance(*nextNode);
			
			nextNode->F = nextNode->G + nextNode->H;

			// �̹� �湮�߰�
			if (g_arrNodes[nextNode->y][nextNode->x] != NULL)
			{
				if (g_arrNodes[nextNode->y][nextNode->x]->pqPop)
				{
					delete nextNode;
					continue;
				}

				// nextNode�� �� �۴ٸ�
				if (g_arrNodes[nextNode->y][nextNode->x]->F > nextNode->F)
				{
					Node *delNode = g_arrNodes[nextNode->y][nextNode->x];
					if (delNode->pqPop)
					{
						int a;
						a = 10;
					}
					delete delNode;
					g_arrNodes[nextNode->y][nextNode->x] = nextNode;
					nextNode->parent = node;
					m_Pq.push(*nextNode);
					InvalidateRect(hWnd, NULL, false);
					UpdateWindow(hWnd);
				}
				continue;
			}
			

			{
				g_arrNodes[nextNode->y][nextNode->x] = nextNode;
				nextNode->parent = node;
				m_Pq.push(*nextNode);
				g_Tile[nextNode->y][nextNode->x] = TILE_TYPE::BLUE;
				InvalidateRect(hWnd, NULL, false);
				UpdateWindow(hWnd);
			}
		}
	}

	PrintRoute(hWnd);
}

void AStar::PrintRoute(HWND hWnd)
{
	if (g_arrNodes[m_destNode->y][m_destNode->x] == NULL)
		return;

	Node *destNode = g_arrNodes[m_destNode->y][m_destNode->x];
	Node *nextNode = destNode->parent;
	while (nextNode != m_startNode)
	{
		g_Tile[nextNode->y][nextNode->x] = TILE_TYPE::PINK;
		nextNode = nextNode->parent;
		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);
	}
}