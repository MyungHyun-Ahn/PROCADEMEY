/*
���� 2. unsigned short (16bit) ������ �� ��Ʈ�� ��Ʈ�� �ϱ�

		- unsigned short ���� = 0 ���� �ʱⰪ ����.
		- Ű����� 1 ~ 16�� ��Ʈ �ڸ� �Է��� ����
		- 1 / 0  �� ����ڷκ��� �޾Ƽ� ������ �ڸ��� ��Ʈ�� 0 �Ǵ� 1�� �ٲ���.

		- �ٸ� ��ü�� �Էµ� ���� �����ʹ� ������ �Ǿ�� ��
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
		printf("���� ����ġ ��ȣ (EXIT -1) : ");
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
			printf("[%d�� ����ġ]\n", i + 1);
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