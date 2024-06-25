#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

struct Test
{

	void TestFunc()
	{
		a = 1;

		a = 2;

		a = 3;
	}

	int a;
	char ch[128];
};

Test testFunc(Test st)
{
	st.ch[0] = 1;

	return st;
}

int main()
{
	Test t1;
	ZeroMemory(&t1, sizeof(t1));
	t1 = testFunc(t1);

	Test *pT = new Test;
	pT->TestFunc();
	return 0;
}