/*
1. 진정한 랜덤

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
	int WinTime;	// 이 아이템이 나올 뽑기 회차
					// 0 이면 일반 아이템
					// 0이 아니면 그 회차에만 나옴
};

st_ITEM g_Gatcha[] = {
			{TEXT("칼"),						20, 0},
			{TEXT("방패"),						20, 0},
			{TEXT("신발"),						20, 0},
			{TEXT("물약"),						20, 0},
			{TEXT("초강력레어무기"),			5, 0},
			{TEXT("초강력방패"),				5, 0},
			{TEXT("초강력레어레어레어신발1"),	1, 50},
			{TEXT("초강력레어레어레어신발2"),	1, 51},
			{TEXT("초강력레어레어레어신발3"),	1, 10}

			// 마지막 3개의 아이템은 일반 확률로는 나오지 않으며
			// 뒤에 입력된 WinTime 회차때만 100% 로 나옴.
};

/*
	## 뽑기 회차 증가. (이는 전역적으로 계산 되어야 함)
	iCount++;

	## 1. 전체 아이템들의 비율 총 합을 구함.
	## 단, WinTime 이 정해진 아이템은 확률의 대상 자체가 아니기 때문에 제외.

	## 2. 본 뽑기 회차에 대한 지정 아이템이 있는지 확인
	## WinTime 과 iCount 가 같은 아이템을 찾는다.
	## 있다면.. 그 아이템을 뽑고 중단.

	## 3. rand() 함수로 확률을 구함
	## 여기서 확률은 1/100 이 아니며, 1/총합비율 임.

	## int iRand = 확률구하기;

	## 4. 전체 아이템 테이블을 돌면서
	## 위에서 구한 Rand 값에 해당 지점의 아이템을 찾는다.

	## 5. 뽑기 회차를 초기화 해야할지 판단하여 초기화.
*/

constexpr int MAX_ITEM_SIZE = 100;
int g_RateTable[MAX_ITEM_SIZE];


// RateTable을 세팅하고 Spatial 아이템의 개수를 반환
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

		// 0 번 인덱스 세팅
		if (i == 0)
		{
			g_RateTable[i] = g_Gatcha[i].Rate;
			continue;
		}

		// 테이블 값 세팅
		g_RateTable[i] = g_RateTable[i - 1] + g_Gatcha[i].Rate;
	}

	return cntSpatial;
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

	// 마지막 회차의 아이템을 뽑으면 초기화
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