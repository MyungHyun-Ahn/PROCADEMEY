/*
1. ������ ����

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

__declspec(naked) WORD RandASM()
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		imul	eax, dword ptr[seedAsm], 343FDh
		add		eax, 269EC3h
		mov		dword ptr[seedAsm], eax
		shr		eax, 10h
		and		eax, 7FFFh
		mov		esp, ebp
		pop		ebp
		ret
	}
}

WORD RandC()
{
	int ret = seedC * 0x343FD;
	ret += 0x269EC3;
	seedC = ret;
	ret = ret >> 0x10;
	ret = ret & 0x7FFF;
	return (WORD)ret;
}

struct st_ITEM
{
	TCHAR Name[40];
	int Rate; // �������� ���� ����
};

st_ITEM g_Gatcha[] = {
			{TEXT("Į"),							20},
			{TEXT("����"),							20},
			{TEXT("�Ź�"),							20},
			{TEXT("����"),							20},
			{TEXT("�ʰ��·����"),				5},
			{TEXT("�ʰ��¹���"),					5},
			{TEXT("�ʰ��·�����Ź�"),		1}
};

/*
	## 1. ��ü �����۵��� ���� �� ���� ����.

	## 2. rand() �Լ��� Ȯ���� ����
	## ���⼭ Ȯ���� 1/100 �� �ƴϸ�, 1/���պ��� ��.

	## int iRand = Ȯ�����ϱ�;


	## 3. ��ü ������ ���̺��� ���鼭
	## ������ ���� Rand ���� �ش� ������ �������� ã�´�.
*/

constexpr int MAX_ITEM_SIZE = 100;
int g_RateTable[MAX_ITEM_SIZE];

int SumRate()
{
	static int i = 0;
	if (i != 0)
		return i;

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

	i = g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1];
	return i;
}

// ���ø� ����
template<int N>
st_ITEM tempGatcha(WORD randVal)
{
	if (randVal > g_RateTable[N - 1] && randVal <= g_RateTable[N])
	{
		return g_Gatcha[N];
	}

	return tempGatcha<N - 1>(randVal);
}

template<>
st_ITEM tempGatcha<0>(WORD randVal)
{
	return g_Gatcha[0]; // 0 ���� ������ �׳� 0�ΰ�
}

int count = 0;

void Gatcha()
{
	int rateSum = SumRate();

	WORD randVal = RandASM() % rateSum + 1;

	int prevVal = 0;

	st_ITEM stItem = tempGatcha<sizeof(g_Gatcha) / sizeof(st_ITEM) - 1>(randVal);

	//for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	//{
	//	if (randVal > prevVal && randVal <= g_RateTable[i]) // �̸� ����ص� ���̺�� ��
	//	{
	//		stItem = g_Gatcha[i];
	//		break;
	//	}

	//	prevVal = g_RateTable[i]; // ���� �� ����
	//}

	_tprintf(TEXT("Num : %3d\tItem : %.40s\t\tRate : %5d / %3d\n"), ++count, stItem.Name, stItem.Rate, g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1]);
}


int _tmain()
{
	_tsetlocale(LC_ALL, TEXT("korean"));

	Srand(time(nullptr));
	// srand(time(nullptr));

	//while (true)
	//{
	//	_gettch();
	//	_tprintf(TEXT("rand() = %d\n"), rand());
	//	_tprintf(TEXT("RandASM() = %d\n"), RandASM());
	//	_tprintf(TEXT("RandC() = %d\n"), RandC());
	//}

	while (true)
	{
		_gettch();
		Gatcha();
	}
}