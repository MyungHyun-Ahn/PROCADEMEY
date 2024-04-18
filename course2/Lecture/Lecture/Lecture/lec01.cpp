#include <stdio.h>

void Test()
{
	int a = 0;
	int b = 0;
	int c = 0;

	a++;
	b++;
	c++;
}

// 저장된 ebp를 건드는 샘플 코드
void TestEbp()
{
	int *p = nullptr;
	int a = 0;
	int b = 0;
	int c = 0;

	int arr[10];

	p = &a;

	// ebp 건들기
	// 전역변수에 있는 값
	//	mov         eax,dword ptr [__security_cookie (0A03000h)]
	//	xor         eax,ebp - 요상한 값
	//	mov         dword ptr [ebp-4],eax
	// 
	// c
	// b
	// a
	// xor ebp
	// oldEbp
	//  
	// 포인터만 선언한 것으로 방지 코드를 넣음
	p += 2;
	*p = 0;

	// return address p+=3;
	p += 1;
	*p = 0;

	// 다시 되돌리기
	//  mov         ecx,dword ptr [ebp-4]  
	//  xor			ecx, ebp
	//  call        __security_check_cookie (0A0107Ch) // 체크
	// 스택 카나리 - 쿠키
	a++;
	b++;
	c++;
}

void A()
{
	int *p = nullptr;
	int a = 0;
	p = &a;
	p += 3;

	*p = 0;
}

void B()
{
	int a = 0;
	a++;

	A();

	a++;
}

void C()
{
	int a = 0;
	a++;

	B();

	a++;
}

int main()
{
	int a = 0;

	A();

	a++;

	return 0;
}