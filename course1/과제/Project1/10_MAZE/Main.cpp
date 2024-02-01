#include <stdio.h>
#include "MazeMap.h"
#include <vector>
#include <deque>
#include <windows.h>

using std::vector;
using std::deque;

void PrintMaze()
{
	for (int y = 0; y < g_MaxY; y++)
	{
		for (int x = 0; x < g_MaxX; x++)
		{
			printf("%c ", g_MazeMap[y][x]);
		}

		printf("\n");
	}
}

bool CanGo(stPos &pos)
{
	if (pos.m_iY < 0 || pos.m_iY >= g_MaxY || pos.m_iX < 0 || pos.m_iX >= g_MaxX)
		return false;

	if (g_Record[pos.m_iY][pos.m_iX].m_bIsVisit)
		return false;

	if (g_MazeMap[pos.m_iY][pos.m_iX] == '#')
		return false;


	return true;
}

void PrintResult()
{
	stRecord record = g_Record[18][18];
	g_MazeMap[18][18] = '*';
	while (record.m_iPrevY != -1 && record.m_iPrevX != -1)
	{
		g_MazeMap[record.m_iPrevY][record.m_iPrevX] = '*';
		record = g_Record[record.m_iPrevY][record.m_iPrevX];
	}

	system("cls");
	PrintMaze();
}

void MazeDFS(int y, int x)
{
	stRecord startRecord;
	startRecord.m_Pos.m_iY = y;
	startRecord.m_Pos.m_iX = x;

	vector<stRecord> vecRecords;
	vecRecords.emplace_back(startRecord);

	while (!vecRecords.empty())
	{
		stRecord curRecord = vecRecords.back();
		vecRecords.pop_back();

		if (!CanGo(curRecord.m_Pos))
			continue;

		system("cls");

		curRecord.m_bIsVisit = true;
		g_Record[curRecord.m_Pos.m_iY][curRecord.m_Pos.m_iX] = curRecord;
		g_MazeMap[curRecord.m_Pos.m_iY][curRecord.m_Pos.m_iX] = '.';

		PrintMaze();
		// Sleep(1000);


	   
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[0]);
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[1]);
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[2]);
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[3]);

	}
	
	PrintResult();
}

void MazeBFS(int y, int x)
{
	stRecord startRecord;
	startRecord.m_Pos.m_iY = y;
	startRecord.m_Pos.m_iX = x;

	deque<stRecord> vecRecords;
	vecRecords.emplace_back(startRecord);

	while (!vecRecords.empty())
	{
		stRecord curRecord = vecRecords.front();
		vecRecords.pop_front();

		if (!CanGo(curRecord.m_Pos))
			continue;

		system("cls");

		curRecord.m_bIsVisit = true;
		g_Record[curRecord.m_Pos.m_iY][curRecord.m_Pos.m_iX] = curRecord;
		g_MazeMap[curRecord.m_Pos.m_iY][curRecord.m_Pos.m_iX] = '.';

		PrintMaze();
		// Sleep(1000);



		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[0]);
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[1]);
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[2]);
		vecRecords.emplace_back(stRecord{ curRecord.m_Pos.m_iY, curRecord.m_Pos.m_iX } + g_Dir[3]);

	}

	PrintResult();
}

int main()
{
	// MazeDFS(1, 1);
	MazeBFS(1, 1);

	return 0;
}