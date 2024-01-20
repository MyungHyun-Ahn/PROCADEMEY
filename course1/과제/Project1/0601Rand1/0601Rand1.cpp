/*
1. 진정한 랜덤

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <windows.h>
#include <locale.h>

#define TEMPLATE

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
		and		eax, 7FFFh
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
	return (WORD)ret;
}

struct st_ITEM
{
	TCHAR Name[40];
	int Rate; // 아이템이 뽑힐 비율
};

st_ITEM g_Gatcha[] = {
			{TEXT("칼"),							20},
			{TEXT("방패"),							20},
			{TEXT("신발"),							20},
			{TEXT("물약"),							20},
			{TEXT("초강력레어무기"),				5},
			{TEXT("초강력방패"),					5},
			{TEXT("초강력레어레어레어신발"),		1}
};

/*
	## 1. 전체 아이템들의 비율 총 합을 구함.

	## 2. rand() 함수로 확률을 구함
	## 여기서 확률은 1/100 이 아니며, 1/총합비율 임.

	## int iRand = 확률구하기;


	## 3. 전체 아이템 테이블을 돌면서
	## 위에서 구한 Rand 값에 해당 지점의 아이템을 찾는다.
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

// 템플릿 버전
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
	return &g_Gatcha[0]; // 0 까지 왔으면 그냥 0인것
}

int count = 0;

void Gatcha()
{
	static int rateSum = 0;
	if (rateSum == 0)
		rateSum = SumRate();

	SHORT randVal = RandASM() % rateSum + 1;

#ifdef TEMPLATE
	st_ITEM *stItem = tempGatcha<sizeof(g_Gatcha) / sizeof(st_ITEM) - 1>(randVal);
#else
	int prevVal = 0;
	st_ITEM *stItem;

	for (int i = 0; i < sizeof(g_Gatcha) / sizeof(st_ITEM); i++)
	{
		if (randVal > prevVal && randVal <= g_RateTable[i]) // 미리 계산해둔 테이블과 비교
		{
			stItem = &g_Gatcha[i];
			break;
		}

		prevVal = g_RateTable[i]; // 이전 값 저장
	}
#endif
	_tprintf(TEXT("Num : %3d\tItem : %.40s\t\tRate : %5d / %3d\n"), ++count, stItem->Name, stItem->Rate, g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1]);
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
		TCHAR ch = _gettch();
		Gatcha();
	}
}