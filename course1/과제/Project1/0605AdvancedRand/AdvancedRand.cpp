#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <time.h>

time_t seedAsm = 1;
time_t seedC = 1;



void Srand(time_t time)
{
	seedAsm = time;
	seedC = time;
}

// 65535
__declspec(naked) WORD AdvancedRandASM()
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		imul	eax, dword ptr[seedAsm], 343FDh
		add		eax, 269EC3h
		mov		dword ptr[seedAsm], eax
		shr		eax, 10h
		;  and eax, 7FFFh
		mov		esp, ebp
		pop		ebp
		ret
	}
}

WORD AdvancedRandC()
{
	time_t ret = seedC * 0x343FD;
	ret += 0x269EC3;
	seedC = ret;
	ret = ret >> 0x10;
	// ret = ret & 0x7FFF;
	return (WORD)ret;
}

union CombinedRand
{
	struct  
	{
		WORD part1;
		WORD part2;
	} SECTION;
	DWORD combinedPart;
};

// 42억까지 나올 수 있는 Rand
DWORD uInt32RandC()
{
	CombinedRand u_Cr;
	u_Cr.SECTION.part1 = AdvancedRandC();
	u_Cr.SECTION.part2 = AdvancedRandC();

	return u_Cr.combinedPart;
}

int main()
{
	while (true)
	{
		TCHAR ch = _gettch();
		_tprintf(TEXT("RandASM() = %d\n"), AdvancedRandASM());
		_tprintf(TEXT("uInt32RandC() = %u\n"), uInt32RandC());
	}
}