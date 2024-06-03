#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <windows.h>
#include "RingBuffer.h"



int main()
{
	srand(time(nullptr));

	RingBuffer rb(1000);
	printf("�׽�Ʈ ���� : %d, %d\n", rb.GetCapacity(), rb.GetFreeSize());

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

		delete pBuffer;

		// �б�
		int sizeBuf;
		rb.Dequeue((char *)& sizeBuf, 4);

		// rand ũ�� ��ŭ �б�
		char randBuffer[1000];
		rb.Dequeue(randBuffer, sizeBuf);

		// Ȯ�ο� putter �б�
		unsigned int putterCheck;
		rb.Dequeue((char *)&putterCheck, 4);
		if (putterCheck == 0xaabbccdd)
		{
			printf("Success\n");
		}
		else
		{
			__debugbreak();
		}
	}
	
	


}