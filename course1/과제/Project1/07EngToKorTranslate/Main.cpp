#include "pch.h"
#include "CDictionary.h"
#include "CTranslator.h"

int _tmain()
{
	_tsetlocale(LC_ALL, TEXT("korean"));

	CTranslator::GetInstance()->Start();

	return 0;
}