#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <process.h>

#define LOOP_COUNT 1000000

int g_Num = 0;


BOOL g_Flags[2];
int g_Turn = 0;

LONG aThreadCnt = 0;
LONG g_Bool = FALSE;
LONG g_Comparand = TRUE;

BOOL g_oldFlags[2];
int g_oldTurn[2];

unsigned ThreadFunc1(LPVOID lpParam)
{
	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		// Lock
		{
			g_Flags[0] = TRUE;
			g_Turn = 0;

			while (1)
			{

				g_oldFlags[1] = g_Flags[1];
				if (g_Flags[1] == FALSE)
					break;

				g_oldTurn[1] = g_Turn;
				if (g_Turn != 0)
					break;
			}
		}

		InterlockedIncrement(&aThreadCnt);
		g_Num++;

		if (aThreadCnt == 2)
		{
			printf("ThreadFunc1 : Flag[1] : %d, oldFlag[1] : %d, Turn : %d : oldTurn[1] : %d\n", g_Flags[1], g_oldFlags[1], g_Turn, g_oldTurn[1]);

			__debugbreak();
		}

		// UnLock
		{
			InterlockedDecrement(&aThreadCnt);
			g_Flags[0] = FALSE;
		}

	}

	return 0;
}

unsigned ThreadFunc2(LPVOID lpParam)
{
	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		// Lock
		{
			g_Flags[1] = TRUE;
			g_Turn = 1;

			while (1)
			{
				g_oldFlags[0] = g_Flags[0];
				if (g_Flags[0] == FALSE)
					break;

				g_oldTurn[0] = g_Turn;
				if (g_Turn != 1)
					break;
			}
		}

		InterlockedIncrement(&aThreadCnt);
		g_Num++;

		if (aThreadCnt == 2)
		{
			printf("ThreadFunc2 : Flag[0] : %d, oldFlag[0] : %d, Turn : %d : oldTurn[0] : %d\n", g_Flags[0], g_oldFlags[0], g_Turn, g_oldTurn[0]);

			__debugbreak();
		}

		// UnLock
		{
			InterlockedDecrement(&aThreadCnt);
			g_Flags[1] = FALSE;
		}

	}

	return 0;
}


int main()
{
	g_Flags[0] = FALSE;
	g_Flags[1] = FALSE;

	HANDLE arrThread[2];

	arrThread[0] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc1, nullptr, CREATE_SUSPENDED, nullptr);
	arrThread[1] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc2, nullptr, CREATE_SUSPENDED, nullptr);

	ResumeThread(arrThread[0]);
	ResumeThread(arrThread[1]);

	WaitForMultipleObjects(2, arrThread, TRUE, INFINITE);

	printf("NUM : %d\n", g_Num);

	CloseHandle(arrThread[0]);
	CloseHandle(arrThread[1]);

	return 0;
}
