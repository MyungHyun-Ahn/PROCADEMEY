#include <stdio.h>
#include <windows.h>
#include <vector>
#include <deque>
#include <windows.h>
#include "CStack.h"
#include "PaintMap.h"


constexpr int g_MaxX = 20;
constexpr int g_MaxY = 20;

char g_ArrMap[g_MaxY][g_MaxX] = {
	{ ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', '0', '0', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', ' ', '0', ' ', '0', '0', ' ', ' ', ' ', '0', '0', ' ', ' ', '0', '0', ' ', ' ', '0', '0' },
	{ '0', '0', ' ', '0', ' ', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0' },
	{ '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', '0', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0' },
	{ '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0' },
	{ ' ', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', ' ', '0', ' ', ' ', '0', '0', ' ', '0' },
	{ ' ', '0', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0', ' ', '0', '0', '0', ' ', '0' },
	{ ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', '0', '0', ' ', ' ' },
	{ ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
};

char g_ArrMap2[g_MaxY][g_MaxX] = {
	{ ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', '0', '0', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', ' ', '0', ' ', '0', '0', ' ', ' ', ' ', '0', '0', ' ', ' ', '0', '0', ' ', ' ', '0', '0' },
	{ '0', '0', ' ', '0', ' ', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0' },
	{ '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', '0', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0' },
	{ '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0' },
	{ ' ', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', ' ', '0', ' ', ' ', '0', '0', ' ', '0' },
	{ ' ', '0', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0', ' ', '0', '0', '0', ' ', '0' },
	{ ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', '0', '0', ' ', ' ' },
	{ ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
};

char g_ArrMap3[g_MaxY][g_MaxX] = {
	{ ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', '0', '0', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', ' ' },
	{ ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0' },
	{ ' ', ' ', ' ', '0', ' ', '0', '0', ' ', ' ', ' ', '0', '0', ' ', ' ', '0', '0', ' ', ' ', '0', '0' },
	{ '0', '0', ' ', '0', ' ', '0', '0', '0', ' ', ' ', '0', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0' },
	{ '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', '0', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0' },
	{ '0', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0' },
	{ ' ', '0', ' ', ' ', ' ', '0', '0', '0', '0', ' ', ' ', '0', ' ', '0', ' ', ' ', '0', '0', ' ', '0' },
	{ ' ', '0', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0', ' ', '0', '0', '0', ' ', '0' },
	{ ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', '0', '0', ' ', ' ' },
	{ ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
};

// 0 : y 1 : x
int dir[4][2] = {
	{  1,  0 }, // DOWN
	{ -1,  0 }, // UP
	{  0,  1 }, // RIGHT
	{  0, -1 }  // LEFT
};


void DFS(int _y, int _x)
{
	// system("cls");

	if (_y < 0 || _y >= g_MaxY || _x < 0 || _x >= g_MaxX)
		return;

	if (g_ArrMap[_y][_x] == '.' || g_ArrMap[_y][_x] == ' ')
		return;

	g_ArrMap[_y][_x] = '.';

	/*for (int y = 0; y < g_MaxY; y++)
	{
		for (int x = 0; x < g_MaxX; x++)
		{
			printf("%c ", g_ArrMap[y][x]);
		}

		printf("\n");
	}
	Sleep(100);*/

	DFS(_y + dir[0][0], _x + dir[0][1]);
	DFS(_y + dir[1][0], _x + dir[1][1]);
	DFS(_y + dir[2][0], _x + dir[2][1]);
	DFS(_y + dir[3][0], _x + dir[3][1]);
	
}



std::vector<std::vector<int>> v;
int DFSVector(int y, int x)
{
	v.push_back({y, x});


	while (!v.empty())
	{
		// system("cls");
		std::vector<int> curDir = v.back();
		v.pop_back();


		if (curDir[0] < 0 || curDir[0] >= g_MaxY || curDir[1] < 0 || curDir[1] >= g_MaxX)
			continue;

		if (g_ArrMap3[curDir[0]][curDir[1]] == '.' || g_ArrMap3[curDir[0]][curDir[1]] == ' ')
			continue;

		g_ArrMap3[curDir[0]][curDir[1]] = '.';

		/*for (int y = 0; y < g_MaxY; y++)
		{
			for (int x = 0; x < g_MaxX; x++)
			{
				printf("%c ", g_ArrMap[y][x]);
			}

			printf("\n");
		}*/

		// Sleep(0);



		v.push_back({ curDir[0] + dir[3][0], curDir[1] + dir[3][1] });
		v.push_back({ curDir[0] + dir[2][0], curDir[1] + dir[2][1] });
		v.push_back({ curDir[0] + dir[1][0], curDir[1] + dir[1][1] });
		v.push_back({ curDir[0] + dir[0][0], curDir[1] + dir[0][1] });
	}

	return 1;
}

struct stDir
{
	int y;
	int x;
};

CStack<stDir> cstack;

void DFSCStack(int y, int x)
{
	stDir startDir{ y, x };
	cstack.Push(startDir);

	while (!cstack.isEmpty())
	{
		// system("cls");
		stDir nDir;
		cstack.Pop(nDir);


		if (nDir.y < 0 || nDir.y >= g_MaxY || nDir.x < 0 || nDir.x >= g_MaxX)
			continue;

		if (g_ArrMap2[nDir.y][nDir.x] == '.' || g_ArrMap2[nDir.y][nDir.x] == ' ')
			continue;

		g_ArrMap2[nDir.y][nDir.x] = '.';

		/*for (int y = 0; y < g_MaxY; y++)
		{
			for (int x = 0; x < g_MaxX; x++)
			{
				printf("%c ", g_ArrMap2[y][x]);
			}

			printf("\n");
		}

		Sleep(0);*/



		cstack.Push({ nDir.y + dir[3][0], nDir.x + dir[3][1] });
		cstack.Push({ nDir.y + dir[2][0], nDir.x + dir[2][1] });
		cstack.Push({ nDir.y + dir[1][0], nDir.x + dir[1][1] });
		cstack.Push({ nDir.y + dir[0][0], nDir.x + dir[0][1] });
	}

}




int main()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	v.reserve(100);
	LARGE_INTEGER vecStart;
	LARGE_INTEGER vecEnd;
	QueryPerformanceCounter(&vecStart);
	DFSVector(10, 10);
	QueryPerformanceCounter(&vecEnd);
	float vecTime = (vecEnd.QuadPart - vecStart.QuadPart) / (float)freq.QuadPart;


	LARGE_INTEGER recursiveStart;
	LARGE_INTEGER recursiveEnd;
	QueryPerformanceCounter(&recursiveStart);
	DFS(10, 10);
	QueryPerformanceCounter(&recursiveEnd);
	float recurTime = (recursiveEnd.QuadPart - recursiveStart.QuadPart) / (float)freq.QuadPart;

	// cstack.Reserve(1000);

	LARGE_INTEGER cstackStart;
	LARGE_INTEGER cstackEnd;
	QueryPerformanceCounter(&cstackStart);
	DFSCStack(10, 10);
	QueryPerformanceCounter(&cstackEnd);
	float cstackTime = (cstackEnd.QuadPart - cstackStart.QuadPart) / (float)freq.QuadPart;

	printf("%f\n", vecTime);
	printf("%f\n", recurTime);
	printf("%f\n", cstackTime);

	for (int y = 0; y < g_MaxY; y++)
	{
		for (int x = 0; x < g_MaxX; x++)
		{
			printf("%c ", g_ArrMap2[y][x]);
		}

		printf("\n");
	}
}