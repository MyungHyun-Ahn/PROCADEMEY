/*
과제 1. unsigned char 변수의 값을 비트단위로 찍어주기

		- 지역변수에 특정 값을 하나 넣음
		- 비트 단위로 분해해서 0 이면 0 출력, 1 이면 1 출력
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <vector>

const unsigned char BIT_MASK = 0b00000001;

int main(void)
{
	unsigned char bit = 0;
	std::vector<unsigned char> backup;

	scanf("%d", &bit);

	for (int i = 0; i < 8; i++)
	{
		unsigned char binaryBit = bit & BIT_MASK;
		bit = bit >> 1;
		backup.push_back(binaryBit);
	}

	while (!backup.empty())
	{
		unsigned char binaryBit = backup.back();
		backup.pop_back();
		printf("%d", binaryBit);
	}

	printf("\n");

	return 0;
}