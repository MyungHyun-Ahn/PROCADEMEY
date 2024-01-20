/*
4. �̱� �Ҷ����� �ش� Ȯ���� -1 �Ͽ�, Ȯ�� ��ü�� �پ��� ���
���� �������� ��í ������ ������ ���� �Ǿ�� �ϸ�, ��� Ȯ���� 0 �� �Ǹ�
���� �����ͷ� �ʱ�ȭ�� �̷�� ���� �Ѵ�
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <windows.h>
#include <locale.h>

time_t seedAsm = 1;
time_t seedC = 1;



void Srand(time_t time)
{
	seedAsm = time;
	seedC = time;
}

__declspec(naked) SHORT RandASM()
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		imul	eax, dword ptr[seedAsm], 343FDh
		add		eax, 269EC3h
		mov		dword ptr[seedAsm], eax
		shr		eax, 10h
		and eax, 7FFFh
		mov		esp, ebp
		pop		ebp
		ret
	}
}

SHORT RandC()
{
	time_t ret = seedC * 0x343FD;
	ret += 0x269EC3;
	seedC = ret;
	ret = ret >> 0x10;
	ret = ret & 0x7FFF;
	return (SHORT)ret;
}

struct st_ITEM
{
	TCHAR Name[40];
	int Rate; // �������� ���� ���� �ϳ� ������ -1
	int Rate_bak; // ���� ���
};

st_ITEM g_Gatcha[] = {
			{TEXT("Į"),						20, 20},
			{TEXT("����"),						20, 20},
			{TEXT("�Ź�"),						20, 20},
			{TEXT("����"),						20, 20},
			{TEXT("�ʰ��·����"),			5, 5},
			{TEXT("�ʰ��¹���"),				5, 5},
			{TEXT("�ʰ��·�����Ź�1"),	1, 1},
			{TEXT("�ʰ��·�����Ź�2"),	1, 1},
			{TEXT("�ʰ��·�����Ź�3"),	1, 1}
};

/*
	���� ���� ������ �̸� ���̺� �����Ͽ�
	���� �˰������� ��� ���������� ���� ���� ���.
*/

constexpr int MAX_ITEM_SIZE = 100;
int g_RateTable[MAX_ITEM_SIZE];


int SumRate()
{
	for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	{
		g_RateTable[i] += g_Gatcha[i].Rate;

		// 0 �� �ε��� ����
		if (i == 0)
		{
			g_RateTable[i] = g_Gatcha[i].Rate;
			continue;
		}

		// ���̺� �� ����
		g_RateTable[i] = g_RateTable[i - 1] + g_Gatcha[i].Rate;
	}

	int ret = g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1];
	return ret;
}

int count = 0;

void ResetRate()
{
	for (st_ITEM &item : g_Gatcha)
	{
		item.Rate = item.Rate_bak;
	}
}

void Gatcha()
{

	SHORT sumRate = SumRate();
	if (sumRate == 0)
	{
		_tprintf(TEXT("Ȯ�� �ʱ�ȭ!\n"));
		ResetRate();
		sumRate = SumRate();
	}


	int prevVal = 0;
	st_ITEM *stItem = nullptr;
	SHORT randVal = RandASM() % sumRate + 1;

	for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	{
		if (randVal > prevVal && randVal <= g_RateTable[i]) // �̸� ����ص� ���̺�� ��
		{
			stItem = &g_Gatcha[i];
			break;
		}

		prevVal = g_RateTable[i]; // ���� �� ����
	}
	
	_tprintf(TEXT("Num : %3d\tItem : %.40s\t\tRate : %5d / %3d\n"), ++count, stItem->Name, stItem->Rate--, g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1]);
}

int _tmain()
{
	_tsetlocale(LC_ALL, TEXT("korean"));

	Srand(time(nullptr));

	while (true)
	{
		TCHAR ch = _gettch();
		Gatcha();
	}
}