#include "pch.h"
#include "LFStack.h"


LFStack<UINT64> s;


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

	for (UINT64 i = 0; i < 25000000; i++)
	{
		UINT64 data;
		s.Pop(&data);
	}

	return 0;
}

#define THREAD_COUNT 4

int main()
{
	HANDLE pushTh = (HANDLE)_beginthreadex(nullptr, 0, ThreadFuncPush, nullptr, CREATE_SUSPENDED, nullptr);
	ResumeThread(pushTh);
	WaitForSingleObject(pushTh, INFINITE); // �־�ΰ�

	// ��� Pop��
	HANDLE arrTh[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		arrTh[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFuncPop, nullptr, CREATE_SUSPENDED, nullptr);
		if (arrTh[i] == 0)
			return 1;
	}

	// HANDLE arrTh[THREAD_COUNT];
	// for (int i = 0; i < THREAD_COUNT; i++)
	// {
	// 	arrTh[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, nullptr, CREATE_SUSPENDED, nullptr);
	// 	if (arrTh[i] == 0)
	// 		return 1;
	// }

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		ResumeThread(arrTh[i]);
	}

	WaitForMultipleObjects(THREAD_COUNT, arrTh, true, INFINITE);

	printf("��������\n");
	
	if (s.m_pTop == nullptr)
		__debugbreak();

	return 0;
}