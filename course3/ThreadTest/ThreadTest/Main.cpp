#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <process.h>

#pragma comment(lib, "winmm.lib")

#define THREAD_COUNT 5

BOOL g_Shutdown = FALSE;
LONG g_connect = 0;

unsigned Accecpt(LPVOID lpParam)
{
	srand((unsigned)time(nullptr));

	while (!g_Shutdown)
	{
		InterlockedIncrement(&g_connect);
		SHORT sleepTime = (rand() % 900) + 100;
		Sleep(sleepTime);
	}

	return 0;
}

unsigned Disconnect(LPVOID lpParam)
{
	srand((unsigned)time(nullptr) + 1);

	while (!g_Shutdown)
	{
		InterlockedDecrement(&g_connect);
		SHORT sleepTime = (rand() % 900) + 100;
		Sleep(sleepTime);
	}

	return 0;
}

CRITICAL_SECTION cs;
SRWLOCK srw;
int g_Data;
unsigned Update(LPVOID lpParam)
{
	DWORD timer = timeGetTime();

	while (!g_Shutdown)
	{
		// EnterCriticalSection(&cs);
		AcquireSRWLockExclusive(&srw);
		++g_Data;

		if (g_Data % 1000 == 0)
			printf("g_Data : %d\n", g_Data);

		ReleaseSRWLockExclusive(&srw);
		// LeaveCriticalSection(&cs);

		Sleep(10);
	}

	return 0;
}

int main()
{
	timeBeginPeriod(1);

	InitializeCriticalSection(&cs);
	InitializeSRWLock(&srw);

	HANDLE arrThread[THREAD_COUNT];

	arrThread[0] = (HANDLE)_beginthreadex(nullptr, 0, Accecpt, NULL, CREATE_SUSPENDED, NULL);
	arrThread[1] = (HANDLE)_beginthreadex(nullptr, 0, Disconnect, NULL, CREATE_SUSPENDED, NULL);
	arrThread[2] = (HANDLE)_beginthreadex(nullptr, 0, Update, NULL, CREATE_SUSPENDED, NULL);
	arrThread[3] = (HANDLE)_beginthreadex(nullptr, 0, Update, NULL, CREATE_SUSPENDED, NULL);
	arrThread[4] = (HANDLE)_beginthreadex(nullptr, 0, Update, NULL, CREATE_SUSPENDED, NULL);

	for (HANDLE th : arrThread)
		ResumeThread(th);

	DWORD oldTime = timeGetTime();
	DWORD count = 0;
	while (count < 20)
	{
		if (DWORD time = timeGetTime(); time - oldTime >= 1000)
		{
			oldTime = time;
			count++;
			printf("Connect : %d\n", g_connect);
		}
		Sleep(1000);
	}

	g_Shutdown = TRUE;

	WaitForMultipleObjects(THREAD_COUNT, arrThread, TRUE, INFINITE);

	timeEndPeriod(1);

	for (HANDLE th : arrThread)
		CloseHandle(th);

	return 0;
}