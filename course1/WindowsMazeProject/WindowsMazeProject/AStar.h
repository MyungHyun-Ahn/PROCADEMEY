#pragma once

extern int startX;
extern int startY;

extern int destX;
extern int destY;

constexpr double Manhattan = 2.0;
constexpr double Euclidean = 1.4;

// y, x
extern int g_Dir[8][2];

struct Node
{
	int y;
	int x;
	double G;
	double H;
	double F;

	Node *parent;

	bool operator<(const Node &other)
	{
		return F < other.F;
	}

	bool operator>(const Node &other)
	{
		return F > other.F;
	}

	bool operator<=(const Node &other)
	{
		return F <= other.F;
	}

	bool operator>=(const Node & other)
	{
		return F >= other.F;
	}

	bool operator==(const Node &other)
	{
		return F == other.F;
	}

	bool operator!=(const Node &other)
	{
		return F != other.F;
	}
};

class AStar
{
public:
	AStar(bool modeG, bool modeH) : m_bModeG(modeG), m_bModeH(modeH)
	{
		ZeroMemory(m_Visited, sizeof(m_Visited));
		{
			m_destNode = new Node;
			m_destNode->x = destX;
			m_destNode->y = destY;
			m_arrNodes[destY][destX] = m_destNode;
		}

		{
			m_Visited[startY][startX] = true;
			m_startNode = new Node;
			m_startNode->x = startX;
			m_startNode->y = startY;
			m_arrNodes[destY][destX] = m_startNode;
			m_startNode->G = 0;
			
			if (m_bModeG)
				m_startNode->H = CalManhattanDistance(*m_startNode);
			else 
				m_startNode->H = CalEuclideanDistance(*m_startNode);

			m_Pq.push(m_startNode);
		}
		
	}
	~AStar() {}

	void Search(HWND hWnd, HDC hdc);

private:
	inline double CalManhattanDistance(Node &node)
	{
		return abs(node.y - m_destNode->y) + abs(node.x - m_destNode->x);
	}

	inline double CalEuclideanDistance(Node &node)
	{
		return sqrt(pow(node.y - m_destNode->y, 2) + pow(node.x - m_destNode->x, 2));
	}

	inline bool CanGo(int nextY, int nextX)
	{
		if (nextX < 0 || nextX >= GRID_WIDTH || nextY < 0 || nextY >= GRID_HEIGHT)
			return false;

		if (g_Tile[nextY][nextX] == TILE_TYPE::OBSTACLE)
			return false;

		return true;
	}

private:
	bool m_bModeG; // 가중치 true : 멘하튼, false : 유클리드
	bool m_bModeH; // 가중치 true : 멘하튼, false : 유클리드

	Node *m_startNode;
	Node *m_destNode;

	std::priority_queue<Node *> m_Pq;
	bool m_Visited[GRID_HEIGHT][GRID_WIDTH];
	Node *m_arrNodes[GRID_HEIGHT][GRID_WIDTH];
};

