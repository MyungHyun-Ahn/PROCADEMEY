/*
4. 뽑기 할때마다 해당 확률을 -1 하여, 확률 자체가 줄어드는 방식
물론 오리지널 가챠 데이터 원본이 보관 되어야 하며, 모든 확률이 0 이 되면
원본 데이터로 초기화가 이루어 져야 한다
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
	int Rate; // 아이템이 뽑힐 비율 하나 뽑히면 -1
	int Rate_bak; // 비율 백업
};

st_ITEM g_Gatcha[] = {
			{TEXT("칼"),						20, 20},
			{TEXT("방패"),						20, 20},
			{TEXT("신발"),						20, 20},
			{TEXT("물약"),						20, 20},
			{TEXT("초강력레어무기"),			5, 5},
			{TEXT("초강력방패"),				5, 5},
			{TEXT("초강력레어레어레어신발1"),	1, 1},
			{TEXT("초강력레어레어레어신발2"),	1, 1},
			{TEXT("초강력레어레어레어신발3"),	1, 1}
};

/*
	나올 랜덤 값들을 미리 테이블에 저장하여
	셔플 알고리즘으로 섞어서 순차적으로 랜덤 값을 사용.
*/

constexpr int MAX_ITEM_SIZE = 100;
int g_RateTable[MAX_ITEM_SIZE];


int SumRate()
{
	for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	{
		g_RateTable[i] += g_Gatcha[i].Rate;

		// 0 번 인덱스 세팅
		if (i == 0)
		{
			g_RateTable[i] = g_Gatcha[i].Rate;
			continue;
		}

		// 테이블 값 세팅
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
		_tprintf(TEXT("확률 초기화!\n"));
		ResetRate();
		sumRate = SumRate();
	}


	int prevVal = 0;
	st_ITEM *stItem = nullptr;
	SHORT randVal = RandASM() % sumRate + 1;

	for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	{
		if (randVal > prevVal && randVal <= g_RateTable[i]) // 미리 계산해둔 테이블과 비교
		{
			stItem = &g_Gatcha[i];
			break;
		}

		prevVal = g_RateTable[i]; // 이전 값 저장
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