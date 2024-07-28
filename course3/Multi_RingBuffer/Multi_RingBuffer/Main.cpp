#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>
#include <process.h>
#include "RingBuffer.h"

RingBuffer rb(1000);

unsigned ConsumerFunc(LPVOID lpParam)
{
	srand(GetCurrentThreadId());

	while (true)
	{
		// buffer ����
		// ����
		// size : 4 , random size (1 - 300), putter : 0xaabbccdd 4����Ʈ

		// 4 �а� randSize ��ŭ �а� 4 ������ 0xaabbccdd
		int randSize = rand() % 300 + 1;
		int bufferSize = 4 + randSize + 4;
		char *pBuffer = new char[bufferSize];
		int *pInt = (int *)pBuffer;
		*pInt = randSize;
		unsigned int putter = 0xaabbccdd;
		memcpy_s(pBuffer + 4 + randSize, 4, &putter, 4);

		rb.Enqueue(pBuffer, bufferSize);

		delete[] pBuffer;
	}

	return 0;
}

int num = 0;

unsigned ProducerFunc(LPVOID lpParam)
{
	while (true)
	{
		// �б�
		int sizeBuf;
		rb.Dequeue((char *)&sizeBuf, 4);

		// rand ũ�� ��ŭ �б�
		char randBuffer[1000];
		rb.Dequeue(randBuffer, sizeBuf);

		// Ȯ�ο� putter �б�
		unsigned int putterCheck;
		int size = rb.Dequeue((char *)&putterCheck, 4);
		if (size != 4)
			__debugbreak();
		if (putterCheck == 0xaabbccdd)
		{
			printf("Success %d\n", num++);
		}
		else
		{
			__debugbreak();
		}
	}

	return 0;
}


int main()
{
	HANDLE handles[2];
	handles[0] = (HANDLE)_beginthreadex(nullptr, 0, ConsumerFunc, nullptr, CREATE_SUSPENDED, nullptr);
	if (handles[0] == 0)
	{
		return 1;
	}
	handles[1] = (HANDLE)_beginthreadex(nullptr, 0, ProducerFunc, nullptr, CREATE_SUSPENDED, nullptr);
	if (handles[1] == 0)
	{
		return 1;
	}

	ResumeThread(handles[0]);
	ResumeThread(handles[1]);

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	return 0;
}