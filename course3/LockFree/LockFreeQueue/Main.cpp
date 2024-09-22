#include "pch.h"
#include "LFQueue.h"

#define THREAD_COUNT 2

LFQueue<int> q;


unsigned int ThreadFunc(LPVOID lpPram)
{
	while (1)
	{
		int thId = GetCurrentThreadId();

		for (int i = 0; i < 3; i++)
		{
			q.Enqueue(i);

		}

		for (int i = 0; i < 3; i++)
		{
			int data;
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
		arrTh[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, nullptr, CREATE_SUSPENDED, nullptr);
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