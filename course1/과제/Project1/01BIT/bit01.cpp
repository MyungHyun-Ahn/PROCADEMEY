/*
���� 1. unsigned char ������ ���� ��Ʈ������ ����ֱ�

		- ���������� Ư�� ���� �ϳ� ����
		- ��Ʈ ������ �����ؼ� 0 �̸� 0 ���, 1 �̸� 1 ���
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