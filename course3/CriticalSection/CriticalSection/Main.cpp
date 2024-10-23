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

#define ADD_COUNT 10000

CRITICAL_SECTION cs;
int g_Val01 = 0;
unsigned ThreadFunc01(void *pParam)
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		EnterCriticalSection(&cs);

		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val01++;
		LeaveCriticalSection(&cs);
	}

	return 0;
}

int g_Val02 = 0;

Mutex mutex(10000);

unsigned ThreadFunc02(void *pParam)
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		mutex.Lock();

		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val02++;
		mutex.UnLock();
	}

	return 0;
}

unsigned RThreadFunc02(void *pParam)
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		mutex.Lock();

		mutex.Lock();

		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val02++;
		mutex.UnLock();
		mutex.UnLock();
	}

	return 0;
}


int g_Val03 = 0;
std::mutex mtx;

unsigned ThreadFunc03(void *pParam)
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		mtx.lock();
		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val03++;
		mtx.unlock();
	}

	return 0;
}

int g_Val04 = 0;
SRWLOCK srw;

unsigned ThreadFunc04(void *pParam)
{
	for (int i = 0; i < ADD_COUNT; i++)
	{
		AcquireSRWLockExclusive(&srw);
		// printf("ThreadID[%d]\n", GetThreadId(GetCurrentThread()));
		g_Val04++;
		ReleaseSRWLockExclusive(&srw);
	}

	return 0;
}

#define THREAD_COUNT 2

void CriticalSectionTest()
{
	// InitializeCriticalSection(&cs);
	InitializeCriticalSectionAndSpinCount(&cs, 10000);
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 0);

		HANDLE thread01[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread01[i] = (HANDLE)_beginthreadex(NULL, NULL, ThreadFunc01, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread01[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread01, TRUE, INFINITE);

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			CloseHandle(thread01[i]);
		}
	}

	printf("%d\n", g_Val01);
	DeleteCriticalSection(&cs);
}

void MyMutexTest()
{
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 0);

		HANDLE thread02[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread02[i] = (HANDLE)_beginthreadex(NULL, NULL, ThreadFunc02, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread02[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread02, TRUE, INFINITE);

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			CloseHandle(thread02[i]);
		}

	}

	printf("%d\n", g_Val02);
}

void StdMutexTest()
{
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 0);

		HANDLE thread03[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread03[i] = (HANDLE)_beginthreadex(NULL, NULL, ThreadFunc03, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread03[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread03, TRUE, INFINITE);

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			CloseHandle(thread03[i]);
		}
	}

	printf("%d\n", g_Val03);
}

void SRWLockTest()
{
	InitializeSRWLock(&srw);

	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 0);

		HANDLE thread04[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread04[i] = (HANDLE)_beginthreadex(NULL, NULL, ThreadFunc04, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread04[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread04, TRUE, INFINITE);

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			CloseHandle(thread04[i]);
		}
	}

	printf("%d\n", g_Val04);
}

void MyRecursionMutexTest()
{
	for (int n = 0; n < 100; ++n)
	{
		PROFILE_BEGIN(__WFUNC__, 0);

		HANDLE thread02[THREAD_COUNT];

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			thread02[i] = (HANDLE)_beginthreadex(NULL, NULL, RThreadFunc02, NULL, CREATE_SUSPENDED, NULL);
		}

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			ResumeThread(thread02[i]);
		}

		WaitForMultipleObjects(THREAD_COUNT, thread02, TRUE, INFINITE);

		for (int i = 0; i < THREAD_COUNT; ++i)
		{
			CloseHandle(thread02[i]);
		}

	}

	printf("%d\n", g_Val02);
}

int main()
{
	// MyRecursionMutexTest();
	MyMutexTest();
	// CriticalSectionTest();
	// SRWLockTest();
	// StdMutexTest();

	return 0;
}