#pragma once

extern int startX;
extern int startY;

extern int destX;
extern int destY;

constexpr double Manhattan = 2.0;
constexpr double Euclidean = 1.41;

// y, x
extern int g_Dir[8][2];

struct Node
{
	bool pqPop = false;
	int y = -1;
	int x = -1;
	double G = -1;
	double H = -1;
	double F = -1;

	Node *parent = NULL;

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

extern Node *curNode;

extern Node *g_arrNodes[GRID_HEIGHT][GRID_WIDTH];

class AStar
{
public:
	AStar(bool modeG, bool modeH) : m_bModeG(modeG), m_bModeH(modeH)
	{
		ZeroMemory(g_arrNodes, sizeof(g_arrNodes));

		{
			m_destNode = new Node;
			m_destNode->x = destX;
			m_destNode->y = destY;
		}

		{
			m_startNode = new Node;
			m_startNode->x = startX;
			m_startNode->y = startY;
			m_startNode->G = 0;

			g_arrNodes[m_startNode->y][m_startNode->x] = m_startNode;
			
			if (m_bModeG)
				m_startNode->H = CalManhattanDistance(*m_startNode);
			else 		   
				m_startNode->H = CalEuclideanDistance(*m_startNode);

			m_startNode->F = m_startNode->G + m_startNode->H;

			m_Pq.push(*m_startNode);
		}
		
	}
	~AStar() 
	{
		delete m_destNode;
	}

	void Search(HWND hWnd, HDC hdc);
	void PrintRoute(HWND hWnd);

	inline bool PriorityQueueIsEmpty() { return m_Pq.empty(); }

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

	std::priority_queue<Node, std::deque<Node>, std::greater<>> m_Pq;
};

