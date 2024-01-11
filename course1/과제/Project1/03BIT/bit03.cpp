/*
과제 3. unsigned int (32bit) 변수를 바이트 단위로 사용하기

		- unsigned int 변수 = 0 초기값 가짐
		- 키보드로 1 ~ 4 의 바이트 위치를 입력 받고
		- 해당 위치에 넣을 데이터 0 ~ 255 를 입력 받음
		- 사용자가 입력한 바이트 위치에 해당 값을 넣음

		- 데이터가 입력 되면 바이트 단위로 쪼개서 출력 & 4바이트 16진수 출력
		- 기존 데이터는 보존이 되어야 하며
		- 입력된 바이트 위치의 데이터는 기존 값을 지우고 넣음.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <vector>

const unsigned int BIT_MASK = 0b00000000000000000000000000000001;

int main(void)
{
	unsigned int bit = 0;
	std::vector<unsigned int> backup;

	while (true)
	{
		int pos;
		printf("바이트 위치 입력 : ");
		scanf("%d", &pos);
		
		// 비트 삭제
		unsigned int delBit = 0b00000000000000000000000011111111;
		delBit = delBit << (pos - 1) * 8;
		delBit = ~delBit;
		bit = bit & delBit;

		unsigned char data;
		printf("데이터 입력 (0 - 255) : ");
		scanf("%d", &data);

		// 비트 삽입
		unsigned int pushBit = 0;
		pushBit = pushBit | data;
		pushBit = pushBit << (pos - 1) * 8;
		bit = bit | pushBit;

		unsigned int tempBit = bit;

		for (int i = 0; i < 32; i++)
		{
			unsigned int biraryBit = tempBit & BIT_MASK;
			backup.push_back(biraryBit);
			tempBit = tempBit >> 1;
		}

		int printCount = 0;
		while (!backup.empty())
		{
			unsigned int biraryBit = backup.back();
			backup.pop_back();

			printf("%d", biraryBit);

			if (++printCount % 8 == 0)
				printf(" ");
		}

		printf("\n");
		printf("%p\n", bit);
	}

	return 0;
}