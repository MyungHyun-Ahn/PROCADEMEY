#pragma once
enum class DIR_MASK
{
	U		= 1,
	D		= 2,
	L		= 4,
	R		= 8,
	UL		= 16,
	UR		= 32,
	DL		= 64,
	DR		= 128
};

extern DIR_MASK g_maskTable[8];

class JPS
{
public:
	JPS(bool modeG, bool modeH) : m_bModeG(modeG), m_bModeH(modeH)
	{
		ZeroMemory(g_arrNodes, sizeof(g_arrNodes));

		{
			m_destNode = new Node;
			m_destNode->x = destX;
			m_destNode->y = destY;
		}

		{
			m_startNode = new Node(startY, startX, 0xFF);
			m_startNode->x = startX;
			m_startNode->y = startY;
			m_startNode->G = 0;
			m_startNode->dir = 0xFF;

			g_arrNodes[m_startNode->y][m_startNode->x] = m_startNode;

			if (m_bModeH)
				m_startNode->H = CalManhattanH(*m_startNode);
			else
				m_startNode->H = CalEuclideanG(*m_startNode);

			m_startNode->F = m_startNode->G + m_startNode->H;

			m_Pq.push(*m_startNode);
		}

	}


	~JPS()
	{
		delete m_destNode;
	}

	void Search(HWND hWnd, HDC hdc);
	void PrintRoute(HWND hWnd);

private:
	inline double CalManhattanG(Node &node)
	{
		return abs(node.y - node.parent->y) + abs(node.x - node.parent->x);
	}

	inline double CalEuclideanG(Node &node)
	{
		return sqrt(pow(node.y - node.parent->y, 2) + pow(node.x - node.parent->x, 2));
	}

	inline double CalManhattanH(Node &node)
	{
		return abs(node.y - m_destNode->y) + abs(node.x - m_destNode->x);
	}

	inline double CalEuclideanH(Node &node)
	{
		return sqrt(pow(node.y - m_destNode->y, 2) + pow(node.x - m_destNode->x, 2));
	}

	inline bool OutMap(int nextY, int nextX)
	{
		if (nextX < 0 || nextX >= GRID_WIDTH || nextY < 0 || nextY >= GRID_HEIGHT)
			return false;

		return true;
	}

	inline bool CheckObstacle(int nextY, int nextX)
	{
		if (g_Tile[nextY][nextX] == TILE_TYPE::OBSTACLE)
			return false;

		return true;
	}

	inline uint8_t CheckCorner(DIR_MASK dirType, int y, int x)
	{
		uint8_t checkBit = 0;

		switch (dirType)
		{
		case DIR_MASK::U:
		{
			if (!CheckObstacle(y, x - 1) && CheckObstacle(y - 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::UL;

			if (!CheckObstacle(y, x + 1) && CheckObstacle(y - 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::UR;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::U;
		}
			break;
		case DIR_MASK::D:
		{
			if (!CheckObstacle(y, x - 1) && CheckObstacle(y + 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::DL;

			if (!CheckObstacle(y, x + 1) && CheckObstacle(y + 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::DR;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::D;
		}
		break;
		case DIR_MASK::L:
		{
			if (!CheckObstacle(y - 1, x) && CheckObstacle(y - 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::UL;

			if (!CheckObstacle(y + 1, x) && CheckObstacle(y + 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::DL;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::L;
		}
		break;
		case DIR_MASK::R:
		{
			if (!CheckObstacle(y - 1, x) && CheckObstacle(y - 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::UR;

			if (!CheckObstacle(y + 1, x) && CheckObstacle(y + 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::DR;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::R;
		}
		break;
		case DIR_MASK::UL:
		{
			if (!CheckObstacle(y, x + 1) && CheckObstacle(y - 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::UR;

			if (!CheckObstacle(y + 1, x) && CheckObstacle(y + 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::DL;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::UL;
		}
		break;
		case DIR_MASK::UR:
		{
			if (!CheckObstacle(y, x - 1) && CheckObstacle(y - 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::UL;

			if (!CheckObstacle(y + 1, x) && CheckObstacle(y + 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::DR;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::UR;
		}
		break;
		case DIR_MASK::DL:
		{
			if (!CheckObstacle(y - 1, x) && CheckObstacle(y - 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::UL;

			if (!CheckObstacle(y, x + 1) && CheckObstacle(y + 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::DR;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::DL;
		}
		break;
		case DIR_MASK::DR:
		{
			if (!CheckObstacle(y - 1, x) && CheckObstacle(y - 1, x + 1))
				checkBit |= (uint8_t)DIR_MASK::UR;

			if (!CheckObstacle(y, x - 1) && CheckObstacle(y + 1, x - 1))
				checkBit |= (uint8_t)DIR_MASK::DL;

			if (checkBit == 0)
				return 0;

			checkBit |= (uint8_t)DIR_MASK::DR;
		}
		}

		return checkBit;
	}

	// 직진하며 벽이 있는지 체크
	uint8_t CheckFront(DIR_MASK dirType, int y, int x);
	bool FrontSearch(DIR_MASK dirType, int y, int x);

private:
	bool m_bModeG; // 가중치 true : 멘하튼, false : 유클리드
	bool m_bModeH; // 가중치 true : 멘하튼, false : 유클리드

	Node *m_startNode;
	Node *m_destNode;

	std::priority_queue<Node, std::deque<Node>, std::greater<>> m_Pq;
};