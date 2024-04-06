#pragma once

extern bool mazeSearchMode;

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
	Node() = default;
	Node(int inY, int inX, uint8_t inDir) : y(inY), x(inX), dir(inDir) {}

	bool pqPop = false;
	uint8_t dir = 0;
	int y = -1;
	int x = -1;
	double G = -1;
	double H = -1;
	double F = -1;

	Node *parent = NULL;
	Node *bresenhamParent = NULL;

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

	bool operator>=(const Node &other)
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
