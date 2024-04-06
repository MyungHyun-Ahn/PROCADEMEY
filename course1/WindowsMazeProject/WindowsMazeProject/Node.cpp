#include "pch.h"
#include "Node.h"

// true - AStar
// false - JPS
bool mazeSearchMode = false;

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

int startX = -1;
int startY = -1;

int destX = -1;
int destY = -1;

Node *curNode = nullptr;
Node *g_arrNodes[GRID_HEIGHT][GRID_WIDTH];