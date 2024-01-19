#include <stdio.h>
#include <stddef.h>



struct stStruct
{
	char a;
	__int64 b;
	short c;
	int d;
	char e;
};

int main()
{
	stStruct st;
	st.a = 0;
	st.b = 0;
	st.c = 0;
	st.d = 0;
	st.e = 0;
	int a = offsetof(stStruct, e);

	return 0;
}