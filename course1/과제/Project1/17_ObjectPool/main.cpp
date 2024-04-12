
#include <iostream>
#include <stdlib.h>
#include "ObjectPool.h"

using namespace std;

class Test
{
public:
	Test()
	{
		cout << "Constructor Test" << endl;
	}

	~Test()
	{
		cout << "Destructor Test" << endl;
	}
};

ObjectPool<Test> objPool(1, true);

int main()
{

	Test *test1 = objPool.Alloc();
	Test *test2 = objPool.Alloc();
	Test *test3 = objPool.Alloc();
	Test *test4 = objPool.Alloc();



	objPool.Free(test1);
	Test *test5 = objPool.Alloc();

	return 0;
}