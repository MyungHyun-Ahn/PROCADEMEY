#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


#include "pch.h"
#include "Core.h"

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(156);
	_CrtSetBreakAlloc(155);
	_CrtSetBreakAlloc(154);
	_CrtSetBreakAlloc(153);

	GameMain();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}