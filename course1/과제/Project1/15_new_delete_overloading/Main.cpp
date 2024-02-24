#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <map>
#include "AllocManager.h"

class A
{
public:
	A() {}
	~A() { printf("class A : ¼Ò¸êÀÚ\n"); }

	int x = 0;
	int y = 0;
};

int main()
{
	int *p1 = new int;
	int *p2 = new int[10];
	A *p3 = new A;
	A *p4 = new A[10];

	int *p5 = nullptr;
	delete p5;
	
	int *p6 = new int;
	delete p6;
	delete p6;

	delete p4;
	

	return 0;
}