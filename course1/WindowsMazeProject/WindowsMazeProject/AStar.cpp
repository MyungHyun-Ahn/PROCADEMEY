#include "pch.h"

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

void AStar::Search(HWND hWnd, HDC hdc)
{
	while (!m_Pq.empty())
	{
		Node *node = m_Pq.top();
		m_Pq.pop();

		g_Tile[node->y][node->x] = TILE_TYPE::YELLOW;

		if (node->y == destY && node->x == destX)
			break;

		for (int i = 0; i < 8; i++)
		{
			int nextX = node->x + g_Dir[i][1];
			int nextY = node->y + g_Dir[i][0];

			if (m_Visited[nextY][nextX])
				continue;

			if (!CanGo(nextY, nextX))
				continue;


			Node *nextNode = new Node;
			m_arrNodes[nextY][nextX] = nextNode;
			nextNode->x = nextX;
			nextNode->y = nextY;

			if (i < 4)
				nextNode->G = node->G + 1;
			else if (m_bModeG)		
				nextNode->G = node->G + Manhattan;
			else						
				nextNode->G = node->G + Euclidean;

			if (m_bModeH)
				nextNode->H = CalManhattanDistance(*nextNode);
			else
				nextNode->H = CalEuclideanDistance(*nextNode);

			nextNode->F = nextNode->G + nextNode->H;

			nextNode->parent = node;

			m_Visited[nextY][nextX] = true;
			m_Pq.push(nextNode);
			g_Tile[nextY][nextX] = TILE_TYPE::BLUE;
		}
	}

	g_Tile[m_startNode->y][m_startNode->x] = TILE_TYPE::GREEN;
	g_Tile[m_destNode->y][m_destNode->x] = TILE_TYPE::RED;
}
