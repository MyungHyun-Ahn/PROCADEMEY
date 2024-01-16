#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

size_t Strlen(const char *_Str)
{
	size_t count = 0;

	for (size_t i = 0; _Str[i] != '\0'; i++)
	{
		count++;
	}

	return count;
}

char *Strcpy(char *_StrDest, const char *_StrSrc)
{
	size_t curser = 0;

	for (curser = 0; _StrSrc[curser] != '\0'; curser++)
	{
		_StrDest[curser] = _StrSrc[curser];
	}

	_StrDest[curser + 1] = '\0';

	return _StrDest;
}

int Strcmp(const char *_Str1, const char *_Str2)
{
	size_t curser = 0;
	do
	{
		if (_Str1[curser] != _Str2[curser])
			return -1;

		curser++;
	} while (_Str1[curser] == '\0' || _Str2[curser] == '\0');

	return 0;
}

char *Strcat(char *strDest, const char *strSrc)
{
	size_t curser = Strlen(strDest);

	for (int i = 0; strSrc[i] != '\0'; i++)
	{
		strDest[curser++] = strSrc[i];
	}
	
	strDest[curser] = '\0';

	return strDest;
}

const char *Strchr(const char *str, int c)
{
	const char *check = NULL;

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == (char)c)
		{
			check = str + i;
		}
	}

	return check;
}

const char *Strstr(const char *str1, const char *str2)
{
	int curser1 = 0;
	int curser2 = 0;
	const char *ptr = NULL;

	while (str1[curser1] != '\0')
	{
		if (str1[curser1] == str2[0]) // 첫 문자를 찾으면
		{
			while (str2[curser2] != '\0')
			{
				if (str1[curser1 + curser2] != str2[curser2])
					break;

				curser2++;
			}

			if (str2[curser2] == '\0') // curser2 가 '\0'이면 일치하는 문자를 찾은 것
			{
				ptr = str1 + curser1;
				break;
			}
		}

		curser1++;
	}

	return ptr;
}

char *Strlwr(char *str)
{
	const unsigned char MASK1 = 0b10000000;
	const unsigned char MASK2 = 0b00100000;
	int curser = 0;

	while (str[curser] != '\0')
	{
		unsigned char c = str[curser] << 2;

		if (MASK1 != (c & MASK1))
			str[curser] = str[curser] | MASK2;
		else
			str[curser] = ~MASK2 & str[curser];

		curser++;
	}

	return str;
}

int Palindrom(const char *str)
{
	int ptr1 = 0;
	int ptr2 = Strlen(str) - 1;

	while (ptr1 <= ptr2)
	{
		if (str[ptr1++] != str[ptr2--])
		{
			return 0;
		}
	}

	return 1;
}

int Fibonacci1(int x)
{
	if (x == 0)
		return 0;

	if (x == 1)
		return 1;

	return Fibonacci1(x - 1) + Fibonacci1(x - 2);
}

int Fibonacci2(int x)
{
	int a0 = 0;
	int a1 = 1;
	int a2;
	for (int i = 2; i <= x; i++)
	{
		a2 = a0 + a1;
		a0 = a1;
		a1 = a2;
	}

	return a2;
}

int fiboArr[50] = { 0, };

int Fibonacci3(int x)
{
	if (x <= 1)
		return x;

	if (fiboArr[x] != 0)
		return fiboArr[x];
	
	fiboArr[x] = Fibonacci3(x - 1) + Fibonacci3(x - 2);
	return fiboArr[x];
}

int main()
{

	char str1[30] = "Hello World ";
	char str2[30] = "Hello ";
	const char *str3 = "World\n";

	size_t size1 = strlen(str1);
	size_t size2 = Strlen(str1);

	printf("%d\n", size1);
	printf("%d\n", size2);

	char *cpy1 = strcpy(str1, str3);
	char *cpy2 = Strcpy(str2, str3);

	printf("%s", cpy1);
	printf("%s", cpy2);

	char str4[30] = "Hello ";
	char str5[30] = "Hello ";
	const char *str6 = "World\n";

	int cmp1 = strcmp(str4, str5);
	int cmp2 = Strcmp(str4, str5);
	int cmp3 = strcmp(str4, str6);
	int cmp4 = Strcmp(str4, str6);

	printf("%d\n", cmp1);
	printf("%d\n", cmp2);
	printf("%d\n", cmp3);
	printf("%d\n", cmp4);

	char str7[30] = "Hello ";
	char str8[30] = "Hello ";
	const char *str9 = "World\n";

	char *cat1 = strcat(str7, str9);
	char *cat2 = Strcat(str8, str9);

	printf("%s", cat1);
	printf("%s", cat2);

	const char *str10 = "abcdefghi";

	const char *chr1 = strchr(str10, 'd');
	const char *chr2 = Strchr(str10, 'd');

	const char *str11 = "I like Apple";
	const char *sstr1 = strstr(str11, "like");
	const char *sstr2 = Strstr(str11, "like");

	char str12[30] = "ABCDEFGHIJKL";
	char *lwr = Strlwr(str12);


	char str13[30] = "ABBA";
	char str14[30] = "AVAVA";
	char str15[30] = "AVVVV";

	int p1 = Palindrom(str13);
	int p2 = Palindrom(str14);
	int p3 = Palindrom(str15);
	

	int x1 = Fibonacci1(10);
	int x2 = Fibonacci2(10);

	fiboArr[0] = 0;
	fiboArr[1] = 1;

	int x3 = Fibonacci3(10);

	return 0;
}