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
	A *p4 = new A[10];
	
	delete[] p4;
	

	return 0;
}