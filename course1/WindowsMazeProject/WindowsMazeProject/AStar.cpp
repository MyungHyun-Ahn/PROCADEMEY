#include "pch.h"
#include "AStar.h"

int g_Dir[8][2] = {
	{-1, 0}, // UP
	{1, 0}, //DOWN
	{0, -1}, // LEFT
	{0, 1}, // RIGHT
	{-1, -1}, // UP_LEFT
	{-1, 1}, // UP_RIGHT
	{1, -1}, // DOWN_LEFT
	{1, 1}, // DOWN_RIGHT
};

int startX;
int startY;

int destX;
int destY;

Node *curNode = nullptr;
Node *g_arrNodes[GRID_HEIGHT][GRID_WIDTH];

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

		// Sleep(500);
		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);

		for (int i = 0; i < 8; i++)
		{
			Node *nextNode = new Node;

			// 다음 좌표
			nextNode->y = node->y + g_Dir[i][0];
			nextNode->x = node->x + g_Dir[i][1];

			if (!CanGo(nextNode->y, nextNode->x))
			{
				// 못 가면 삭제
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

			// 이미 방문했고
			if (g_arrNodes[nextNode->y][nextNode->x] != NULL)
			{
				// nextNode가 더 작다면
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
			}
			else
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