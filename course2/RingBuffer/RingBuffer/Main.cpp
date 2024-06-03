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
	printf("테스트 시작 : %d, %d\n", rb.GetCapacity(), rb.GetFreeSize());

	while (true)
	{
		// buffer 생성
		// 구조
		// size : 4 , random size (1 - 300), putter : 0xaabbccdd 4바이트

		// 4 읽고 randSize 만큼 읽고 4 읽으면 0xaabbccdd
		int randSize = rand() % 300 + 1;
		int bufferSize = 4 + randSize + 4;
		char *pBuffer = new char[bufferSize];
		int *pInt = (int *)pBuffer;
		*pInt = randSize;
		unsigned int putter = 0xaabbccdd;
		memcpy_s(pBuffer + 4 + randSize, 4, &putter, 4);

		rb.Enqueue(pBuffer, bufferSize);

		delete pBuffer;

		// 읽기
		int sizeBuf;
		rb.Dequeue((char *)& sizeBuf, 4);

		// rand 크기 만큼 읽기
		char randBuffer[1000];
		rb.Dequeue(randBuffer, sizeBuf);

		// 확인용 putter 읽기
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