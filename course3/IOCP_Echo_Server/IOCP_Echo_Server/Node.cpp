#include "pch.h"
#include "Node.h"

BOOL CheckZeroBitCount()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	// 최상단 포인터
	ULONG_PTR max = (ULONG_PTR)si.lpMaximumApplicationAddress + (ULONG_PTR)si.lpMinimumApplicationAddress;

	// 비트 연산
	max = (~ADDR_MASK) & max;

	if (max != 0)
		return FALSE;

	return TRUE;
}

ULONG_PTR GetNodePtr(ULONG_PTR ptr)
{
	ULONG_PTR addr = ptr & ADDR_MASK;
	return addr;
}

ULONG_PTR GetNodeIdentifier(ULONG_PTR ptr)
{
	ULONG_PTR identifier = ptr & IDENTIFIER_MASK;
	identifier = identifier >> NON_ZERO_BIT;
	return identifier;
}

ULONG_PTR CombinePtrAndIdnetifier(ULONG_PTR ident, ULONG_PTR ptr)
{
	ident = ident << NON_ZERO_BIT;
	return ptr | ident;
}