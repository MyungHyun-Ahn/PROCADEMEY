#include <stdio.h>
#include <memory>
#include "SerializableBuffer.h"

int main()
{
	SerializableBuffer buffer;

	int a = 1234;
	short b = 12;

	long c = 4321;

	float d = 1.123f;

	__int64 e = 12345678;

	double f = 1.12345678;

	buffer << a << b << c << d << e << f;

	int g;
	short h;
	long i;
	float j;
	__int64 k;
	double l;

	buffer >> g >> h >> i >> j >> k >> l;

	printf("%d\n", g);
	printf("%d\n", h);
	printf("%d\n", i);
	printf("%f\n", j);
	printf("%lld\n", k);
	printf("%lf", l);
	
}