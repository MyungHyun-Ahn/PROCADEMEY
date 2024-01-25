#include "pch.h"
#include "CGameEngine.h"

int _tmain()
{
	_tsetlocale(LC_ALL, TEXT("korean"));
	CGameEngine::GetInstance()->Start();
}