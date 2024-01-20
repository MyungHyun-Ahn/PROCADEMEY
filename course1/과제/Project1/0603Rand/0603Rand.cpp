/*
3. 랜덤 테이블

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
	int Rate; // 아이템이 뽑힐 비율
};

st_ITEM g_Gatcha[] = {
			{TEXT("칼"),						20},
			{TEXT("방패"),						20},
			{TEXT("신발"),						20},
			{TEXT("물약"),						20},
			{TEXT("초강력레어무기"),			5},
			{TEXT("초강력방패"),				5},
			{TEXT("초강력레어레어레어신발1"),	1},
			{TEXT("초강력레어레어레어신발2"),	1},
			{TEXT("초강력레어레어레어신발3"),	1}
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

SHORT g_ArrIndex[MAX_ITEM_SIZE];

void Swap(SHORT &a, SHORT &b)
{
	SHORT temp = a;
	a = b;
	b = temp;
}

void IndexArrInit(SHORT n)
{
	for (SHORT i = 1; i <= n; i++)
	{
		g_ArrIndex[i - 1] = i;
	}
}

void Shuffle(short n)
{
	// high index ->  0
	for (SHORT i = n - 1; i >= 0; i--)
	{
		// 0 <= selIndex <= i인 난수 생성
		SHORT selIndex = RandASM() % (i + 1);
		// Swap
		Swap(g_ArrIndex[i], g_ArrIndex[selIndex]);
	}
}

int count = 0;

void Gatcha()
{
	static SHORT sumRate = 0;
	if (sumRate == 0)
		sumRate = SumRate();
	static bool flag = true;
	if (flag)
	{
		_tprintf(TEXT("갓챠 테이블 셔플!\n"));
		IndexArrInit(sumRate);
		Shuffle(sumRate);
		flag = false;
	}


	st_ITEM *item = tempGatcha<sizeof(g_Gatcha) / sizeof(st_ITEM) - 1>(g_ArrIndex[count++]);
	_tprintf(TEXT("Num : %3d\tItem : %.40s\t\tRate : %5d / %3d\n"), count, item->Name, item->Rate, g_RateTable[sizeof(g_Gatcha) / sizeof(st_ITEM) - 1]);

	if (count % sumRate == 0)
		flag = true;
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