/*
���� 3. unsigned int (32bit) ������ ����Ʈ ������ ����ϱ�

		- unsigned int ���� = 0 �ʱⰪ ����
		- Ű����� 1 ~ 4 �� ����Ʈ ��ġ�� �Է� �ް�
		- �ش� ��ġ�� ���� ������ 0 ~ 255 �� �Է� ����
		- ����ڰ� �Է��� ����Ʈ ��ġ�� �ش� ���� ����

		- �����Ͱ� �Է� �Ǹ� ����Ʈ ������ �ɰ��� ��� & 4����Ʈ 16���� ���
		- ���� �����ʹ� ������ �Ǿ�� �ϸ�
		- �Էµ� ����Ʈ ��ġ�� �����ʹ� ���� ���� ����� ����.
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
		printf("����Ʈ ��ġ �Է� : ");
		scanf("%d", &pos);
		
		// ��Ʈ ����
		unsigned int delBit = 0b00000000000000000000000011111111;
		delBit = delBit << (pos - 1) * 8;
		delBit = ~delBit;
		bit = bit & delBit;

		unsigned char data;
		printf("������ �Է� (0 - 255) : ");
		scanf("%d", &data);

		// ��Ʈ ����
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