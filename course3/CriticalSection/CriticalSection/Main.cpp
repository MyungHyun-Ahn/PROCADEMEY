#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <string>
#include <map>
#include <ctime>
#include <tchar.h>
#include <mutex>
#include "Mutex.h"
#include "Profiler.h"

#pragma comment(lib, "Synchronization.lib")

#define ADD_COUNT 50000

CRITICAL_SECTION cs;
int g_Val01 = 0;
void ThreadFunc01()
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		EnterCriticalSection(&cs);

		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val01++;
		LeaveCriticalSection(&cs);
	}
}

int g_Val02 = 0;

Mutex mutex(10000);

void ThreadFunc02()
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		mutex.Lock();

		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val02++;
		mutex.UnLock();
	}
}

int g_Val03 = 0;
std::mutex mtx;

void ThreadFunc03()
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		mtx.lock();
		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val03++;
		mtx.unlock();
	}
}

int g_Val04 = 0;
SRWLOCK srw;

void ThreadFunc04()
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		AcquireSRWLockExclusive(&srw);
		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val04++;
		ReleaseSRWLockExclusive(&srw);
	}
}

#define THREAD_COUNT 4

int main()
{
	InitializeCriticalSection(&cs);
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 1);

		HANDLE thread01[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread01[i] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadFunc01, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread01[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread01, TRUE, INFINITE);

		// printf("%d\n", g_Val01);
	}
	DeleteCriticalSection(&cs);
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 2);

		HANDLE thread02[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread02[i] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadFunc02, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread02[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread02, TRUE, INFINITE);

		// printf("%d\n", g_Val02);
	}
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 3);

		HANDLE thread03[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread03[i] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadFunc03, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread03[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread03, TRUE, INFINITE);

		// printf("%d\n", g_Val03);
	}

	InitializeSRWLock(&srw);

	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 4);

		HANDLE thread04[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread04[i] = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadFunc04, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread04[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread04, TRUE, INFINITE);

		// printf("%d\n", g_Val03);
	}

	return 0;
}