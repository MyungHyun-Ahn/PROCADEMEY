#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "CacheSimulator.h"

#pragma warning(disable:6066)
#pragma warning(disable:4477)

struct Test
{
	alignas(64)
		int num1;
	int num2;
	int num3;
	int num4;
	int num5;
	int num6;
	int num7;
	int num8;
};
int main()
{
	CacheSimulator cs;

	int a;
	UINT_PTR addr = (UINT_PTR)&a;
	int offset = 4096;
	int index = GetCacheIndex(addr);
	printf("index = %d, 8Way\n", index);

	bool isSuccess = cs.IsCacheHit((void *)addr);
	if (isSuccess)
		printf("1.Cache hit, %p,%p\n", addr, GetCacheTag(addr));
	else
		printf("1.Cache miss, %p,%p\n", addr, GetCacheTag(addr));

	for (int i = 2; i < 9; ++i)
	{
		addr += offset;
		isSuccess = cs.IsCacheHit((void *)addr);
		if (isSuccess)
			printf("%d.Cache hit, %p,%p\n", i, addr, GetCacheTag(addr));
		else
			printf("%d.Cache miss, %p,%p\n", i, addr, GetCacheTag(addr));
	}
	UINT_PTR temp = addr;

	addr = (UINT_PTR)&a;
	for (int i = 1; i <= 9; ++i)
	{
		isSuccess = cs.IsCacheHit((void *)addr);
		if (isSuccess)
			printf("%d.Cache hit, %p,%p\n", i, addr, GetCacheTag(addr));
		else
			printf("%d.Cache miss, %p,%p\n", i, addr, GetCacheTag(addr));
		addr += offset;
	}

	addr = (UINT_PTR)&a;
	addr += offset;

	for (int i = 2; i <= 8; ++i)
	{
		isSuccess = cs.IsCacheHit((void *)addr);
		if (isSuccess)
			printf("%d.Cache hit, %p,%p\n", i, addr, GetCacheTag(addr));
		else
			printf("%d.Cache miss, %p,%p\n", i, addr, GetCacheTag(addr));
		addr += offset;
	}

	printf("\n");
	Test test;
	addr = (UINT_PTR)&test.num1;
	isSuccess = cs.IsCacheHit((void *)addr);
	if (isSuccess)
		printf("1.Cache hit, %p,%p,%d,%p\n", addr, GetCacheTag(addr), GetCacheIndex(addr), addr & 0x3F);
	else
		printf("1.Cache miss, %p,%p,%d,%p\n", addr, GetCacheTag(addr), GetCacheIndex(addr), addr & 0x3F);

	for (int i = 2; i <= 32; ++i)
	{
		addr += sizeof(int);
		isSuccess = cs.IsCacheHit((void *)addr);
		if (isSuccess)
			printf("%d.Cache hit, %p,%p,%d,%p\n", i, addr, GetCacheTag(addr), GetCacheIndex(addr), addr & 0x3F);
		else
			printf("%d.Cache miss, %p,%p,%d,%p\n", i, addr, GetCacheTag(addr), GetCacheIndex(addr), addr & 0x3F);
	}



}
