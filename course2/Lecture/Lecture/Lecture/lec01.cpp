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

// ����� ebp�� �ǵ�� ���� �ڵ�
void TestEbp()
{
	int *p = nullptr;
	int a = 0;
	int b = 0;
	int c = 0;

	int arr[10];

	p = &a;

	// ebp �ǵ��
	// ���������� �ִ� ��
	//	mov         eax,dword ptr [__security_cookie (0A03000h)]
	//	xor         eax,ebp - ����� ��
	//	mov         dword ptr [ebp-4],eax
	// 
	// c
	// b
	// a
	// xor ebp
	// oldEbp
	//  
	// �����͸� ������ ������ ���� �ڵ带 ����
	p += 2;
	*p = 0;

	// return address p+=3;
	p += 1;
	*p = 0;

	// �ٽ� �ǵ�����
	//  mov         ecx,dword ptr [ebp-4]  
	//  xor			ecx, ebp
	//  call        __security_check_cookie (0A0107Ch) // üũ
	// ���� ī���� - ��Ű
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