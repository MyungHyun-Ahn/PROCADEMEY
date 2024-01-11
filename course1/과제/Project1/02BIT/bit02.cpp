/*
과제 2. unsigned short (16bit) 변수의 각 비트를 컨트롤 하기

		- unsigned short 변수 = 0 으로 초기값 가짐.
		- 키보드로 1 ~ 16의 비트 자리 입력을 받음
		- 1 / 0  을 사용자로부터 받아서 지정된 자리의 비트를 0 또는 1로 바꿔줌.

		- 다른 위체이 입력된 기존 데이터는 보존이 되어야 함
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

const unsigned short BIT_MASK01 = 0b1111111111111111;
const unsigned short BIT_MASK02 = 0b0000000000000001;


int main(void)
{
	unsigned short bit = 0;

	while (true)
	{
		int bitPos;
		unsigned short selectBit = 0b0000000000000001;
		printf("조작 스위치 번호 (EXIT -1) : ");
		scanf("%d", &bitPos);
		
		if (bitPos == -1)
			break;

		if (!(bitPos <= 16 && bitPos >= 0))
			break;
			

		selectBit = selectBit << bitPos - 1;

		int sel;
		printf("ON 1, OFF 2 : ");
		scanf("%d", &sel);

		if (sel == 1)
			bit = bit | selectBit;
		
		if (sel == 2)
			bit = (BIT_MASK01 ^ (selectBit)) & bit;

		unsigned short tempBit = bit;

		for (int i = 0; i < 16; i++)
		{
			printf("[%d번 스위치]\n", i + 1);
			if (tempBit & BIT_MASK02)
				printf("[ ON ]\n");
			else
				printf("[ OFF ]\n");
			tempBit = tempBit >> 1;
		}

		while (getchar() != '\n');
	}

	return 0;
}