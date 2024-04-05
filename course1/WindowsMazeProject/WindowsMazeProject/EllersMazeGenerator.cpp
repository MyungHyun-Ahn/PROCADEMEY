#include "pch.h"
#include "EllersMazeGenerator.h"

int g_Groups[GRID_HEIGHT][GRID_WIDTH];

void EllersMazeGenerator::Generator()
{

	srand(time(nullptr));

	ZeroMemory(g_Groups, sizeof(g_Groups));

	int group = 1;

	for (int x = 1; x < GRID_WIDTH - 1; x += 2)
	{
		g_Groups[1][x] = group;

		group++;
	}
	
	for (int x = 1; x < GRID_WIDTH - 3; x++)
	{
		// 벽이면 패스
		if (g_Groups[1][x] == 0)
			continue;

		if (g_Groups[1][x] == g_Groups[1][x + 2])
			continue;

		// 합칠지 말지 판단
		short num = rand() % 2;

		// 합치지 않기
		if (num)
			continue;

		g_Groups[1][x + 1] = g_Groups[1][x];
		g_Groups[1][x + 2] = g_Groups[1][x];
	}

	for (int y = 2; y < GRID_HEIGHT; y+=2)
	{
		// 밑으로
		int groupCount = 1;
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			if (g_Groups[y - 2][x] != 0)
			{
				groupCount++;
				continue;
			}

			if(groupCount == 0)
				continue;

			// 밑으로 뚫을지 말지 판단
			short num = rand() % groupCount + 1;

			g_Groups[y - 1][x - groupCount + num - 1] = g_Groups[y - 2][x - 1];
			g_Groups[y][x - groupCount + num - 1] = g_Groups[y - 2][x - 1];
			groupCount = 1;
		}

		// 그룹 할당
		for (int x = 1; x < GRID_WIDTH - 2; x++)
		{
			if (g_Groups[y][x] != 0)
				continue;

			// 0 인 상황
			g_Groups[y][x + 1] = group;
			group++;
		}

		// 그룹 합치기
		for (int x = 0; x < GRID_WIDTH - 3; x++)
		{
			// 벽이면 패스
			if (g_Groups[y][x] == 0)
				continue;

			// 합칠지 말지 판단
			short num = rand() % 2;

			// 합치지 않기
			if (num)
				continue;

			g_Groups[y][x + 1] = g_Groups[y][x];
			g_Groups[y][x + 2] = g_Groups[y][x];
		}
	}



	for (int y = 0; y < GRID_HEIGHT; y++)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			if (g_Groups[y][x] != 0)
				continue;

			g_Tile[y][x] = TILE_TYPE::OBSTACLE;
		}
	}
}
