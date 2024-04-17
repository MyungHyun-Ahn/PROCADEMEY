
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <map>
#include <string>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include "CProfiler.h"
#include "ObjectPool.h"

using namespace std;

class Test
{
public:
	Test()
	{
		// cout << "Constructor Test" << endl;
	}

	~Test()
	{
		// cout << "Destructor Test" << endl;
	}

	char byte[1024] = {0, };
};

void Test1()
{
	Test *test[100000];

	PROFILE_BEGIN(__WFUNC__, 1);

	for (int i = 0; i < 1024; i++)
	{
		test[i] = new Test;
	}

	for (int i = 0; i < 1024; i++)
	{
		delete test[i];
	}
}

ObjectPool<Test> objPool(0, true);

void Test2()
{
	Test *test[100000];
	PROFILE_BEGIN(__WFUNC__, 2);

	for (int i = 0; i < 1024; i++)
	{
		test[i] = objPool.Alloc();
	}

	for (int i = 0; i < 1024; i++)
	{
		objPool.Free(test[i]);
	}
}

int main()
{
	for (int i = 0; i < 200; i++)
	{
		Test1();
		Test2();
	}

	return 0;
}