#include "pch.h"

#include "LFMemoryPool.h"
#include "LFStack.h"
#include "LFQueue.h"

#define THREAD_COUNT 2

LFStack<UINT64> s;
LFQueue<UINT64> q;

unsigned int ThreadFunc(LPVOID lpPram)
{
	while (1)
	{
		int thId = GetCurrentThreadId();

		for (UINT64 i = 0; i < 1000; i++)
		{
			s.Push(i);

		}

		for (int i = 0; i < 1000; i++)
		{
			UINT64 data;
			s.Pop(&data);
			// printf("Pop ThreadId = %d, Data = %d\n", thId, data);
		}
	}
}

unsigned int ThreadFuncPush(LPVOID lpPram)
{

	int thId = GetCurrentThreadId();

	for (UINT64 i = 0; i < 100000000; i++)
	{
		s.Push(i);
	}

	return 0;
}

unsigned int ThreadFuncPop(LPVOID lpPram)
{

	int thId = GetCurrentThreadId();

	for (UINT64 i = 0; i < 100000000 / THREAD_COUNT; i++)
	{
		UINT64 data;
		s.Pop(&data);
	}

	return 0;
}

unsigned int ThreadFuncQueue(LPVOID lpPram)
{
	while (1)
	{
		int thId = GetCurrentThreadId();

		for (UINT64 i = 0; i < 3; i++)
		{
			q.Enqueue(i);

		}

		for (int i = 0; i < 3; i++)
		{
			UINT64 data;
			q.Dequeue(&data);
			// printf("Pop ThreadId = %d, Data = %d\n", thId, data);
		}
	}
}

int main()
{
	HANDLE arrTh[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		arrTh[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFuncQueue, nullptr, CREATE_SUSPENDED, nullptr);
		if (arrTh[i] == 0)
			return 1;
	}

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		ResumeThread(arrTh[i]);
	}

	WaitForMultipleObjects(THREAD_COUNT, arrTh, true, INFINITE);

	printf("정상종료\n");

	return 0;
}