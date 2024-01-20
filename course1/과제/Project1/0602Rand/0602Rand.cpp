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
	int Rate; // �������� ���� ����
	int WinTime;	// �� �������� ���� �̱� ȸ��
					// 0 �̸� �Ϲ� ������
					// 0�� �ƴϸ� �� ȸ������ ����
};

st_ITEM g_Gatcha[] = {
			{TEXT("Į"),						20, 0},
			{TEXT("����"),						20, 0},
			{TEXT("�Ź�"),						20, 0},
			{TEXT("����"),						20, 0},
			{TEXT("�ʰ��·����"),			5, 0},
			{TEXT("�ʰ��¹���"),				5, 0},
			{TEXT("�ʰ��·�����Ź�1"),	1, 50},
			{TEXT("�ʰ��·�����Ź�2"),	1, 51},
			{TEXT("�ʰ��·�����Ź�3"),	1, 10}

			// ������ 3���� �������� �Ϲ� Ȯ���δ� ������ ������
			// �ڿ� �Էµ� WinTime ȸ������ 100% �� ����.
};

/*
	## �̱� ȸ�� ����. (�̴� ���������� ��� �Ǿ�� ��)
	iCount++;

	## 1. ��ü �����۵��� ���� �� ���� ����.
	## ��, WinTime �� ������ �������� Ȯ���� ��� ��ü�� �ƴϱ� ������ ����.

	## 2. �� �̱� ȸ���� ���� ���� �������� �ִ��� Ȯ��
	## WinTime �� iCount �� ���� �������� ã�´�.
	## �ִٸ�.. �� �������� �̰� �ߴ�.

	## 3. rand() �Լ��� Ȯ���� ����
	## ���⼭ Ȯ���� 1/100 �� �ƴϸ�, 1/���պ��� ��.

	## int iRand = Ȯ�����ϱ�;

	## 4. ��ü ������ ���̺��� ���鼭
	## ������ ���� Rand ���� �ش� ������ �������� ã�´�.

	## 5. �̱� ȸ���� �ʱ�ȭ �ؾ����� �Ǵ��Ͽ� �ʱ�ȭ.
*/

constexpr int MAX_ITEM_SIZE = 100;
int g_RateTable[MAX_ITEM_SIZE];


// RateTable�� �����ϰ� Spatial �������� ������ ��ȯ
SHORT SumRate()
{
	SHORT cntSpatial = 0;
	for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	{
		if (g_Gatcha[i].WinTime != 0)
		{
			cntSpatial++;
			continue;
		}

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

	return cntSpatial;
}

// ���ø� ����
template<SHORT N>
st_ITEM *tempGatcha(SHORT randVal)
{
	if (randVal > g_RateTable[N - 1] && randVal <= g_RateTable[N])
	{
		return &g_Gatcha[N];
	}

	return tempGatcha<N - 1>(randVal);
}

template<>
st_ITEM *tempGatcha<0>(SHORT randVal)
{
	return &g_Gatcha[0]; // 0 ���� ������ �׳� 0�ΰ�
}

int count = 0;

void Gatcha()
{
	static SHORT cntSpatial = -1;
	static SHORT maxRate = -1;
	if (cntSpatial == -1)
	{
		cntSpatial = SumRate();
		maxRate = g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1 - cntSpatial];
	}

	count++;
	st_ITEM* item = nullptr;
	do 
	{
		for (SHORT i = sizeof(g_Gatcha) / sizeof(st_ITEM) - cntSpatial; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
		{
			if (g_Gatcha[i].WinTime == count)
			{
				item = &g_Gatcha[i];
				break;
			}
		}

		if (item != nullptr)
			break;

		SHORT randVal = RandASM() % maxRate + 1;
		item = tempGatcha<sizeof(g_Gatcha) / sizeof(st_ITEM) - 1>(randVal);

	} while (0);

	_tprintf(TEXT("Num : %3d\tItem : %.40s\t\tRate : %5d / %3d\n"), count, item->Name, item->Rate, g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1 - cntSpatial]);

	// ������ ȸ���� �������� ������ �ʱ�ȭ
	if (count == 51)
		count = 0;
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