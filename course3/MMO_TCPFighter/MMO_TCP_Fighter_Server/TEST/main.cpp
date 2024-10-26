#include <stdio.h>
#include <string>
#include <map>
#include <tchar.h>
#include <time.h>
#include "windows.h"
#include "CProfiler.h"
#include <unordered_map>

std::unordered_map<INT, INT> unMap;

// TEST01
void stdAdvance01()
{
	PROFILE_BEGIN(__WFUNC__, 0);

	int loopCount = 0;

	while (true)
	{
		auto startIt1 = unMap.begin();
		auto endIt = startIt1;
		std::advance(startIt1, loopCount * 63);
		auto startIt2 = startIt1; // 백업용 또 써야하니깐
		std::advance(endIt, (loopCount + 1) * 63);

		for (; startIt1 != endIt; ++startIt1)
		{
			if (startIt1 == unMap.end())
				break;
			// 등록 작업
		}

		startIt1 = startIt2;
		for (; startIt1 != endIt; ++startIt1)
		{
			if (startIt1 == unMap.end())
				break;

			// Recv

			// Send
		}

		if (startIt1 == unMap.end())
			break;

		loopCount++;
	}
}

// TEST02
void stdAdvance02()
{
	PROFILE_BEGIN(__WFUNC__, 0);

	int loopCount = 0;

	auto startIt1 = unMap.begin();
	auto endIt = startIt1;
	auto startIt2 = startIt1;
	std::advance(endIt, 63);

	while (true)
	{
		startIt1 = startIt2;
		for (; startIt1 != endIt; ++startIt1)
		{
			if (startIt1 == unMap.end())
				break;
			// 등록 작업
		}

		startIt1 = startIt2;
		for (; startIt1 != endIt; ++startIt1)
		{
			if (startIt1 == unMap.end())
				break;

			// Recv

			// Send
		}

		std::advance(startIt2, 63);
		std::advance(endIt, 63);

		if (startIt1 == unMap.end())
			break;

		loopCount++;
	}
}

// TEST03
void stdAdvance03()
{
	PROFILE_BEGIN(__WFUNC__, 0);

	int loopCount = 0;

	auto startIt1 = unMap.begin();
	auto endIt = startIt1;
	auto startIt2 = startIt1;
	for (int i = 0; i < 63; i++)
	{
		++endIt;
		if (endIt == unMap.end())
			break;
	}

	while (true)
	{
		startIt1 = startIt2;
		for (; startIt1 != endIt; ++startIt1)
		{
			if (startIt1 == unMap.end())
				break;
			// 등록 작업
		}

		startIt1 = startIt2;
		for (; startIt1 != endIt; ++startIt1)
		{
			if (startIt1 == unMap.end())
				break;

			// Recv

			// Send
		}

		for (int i = 0; i < 63; i++)
		{
			++startIt2;
			++endIt;

			if (endIt == unMap.end())
				break;
		}

		if (startIt1 == unMap.end())
			break;

		loopCount++;
	}
}

int main()
{
	for (int i = 0; i < 10000; i++)
	{
		unMap.insert(std::make_pair(i, i));
	}


	for (int i = 0; i < 10; i++)
	{
		stdAdvance01();

		stdAdvance02();

		stdAdvance03();
	}

	return 0;
}