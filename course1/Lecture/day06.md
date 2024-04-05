# Day 06

## Rand 난수
* 정교하지 않다. = 분포가 균등하지 않다.
* 정해진 범위 안에서 균등하게 나온다.
* 순서만 뒤집어져서 나온다.

rand() : 의사 난수를 생성
* 안전한 버전 : rand_s();
* \<random> : C++ 표준 라이브러리
* 0 ~ signed short 최대값 65535
* 기본 시드값으로 두었을 때 : 항상 일정한 값을 보인다.
* 결국 계산식

srand(시드값);
* 보통 시드값에서는 시간을 많이 넣는다.
* srand(time(nullptr));
  * 프로그램을 켤때마다 시간이 바뀐다.
  * 시작값이 거의 비슷하다.

TIMESTAMP
* 64비트로 바뀌었다.
* 게시글, 컨텐츠에서 많이 쓴다.

TIMESTAMP를 쓰는 이유
* 연월일시초 -> 나누어서 관리
* 1초를 더했을 때 어떻게 변하는지 모른다.
* 달력을 먼저 구현해야할 수 있다.
* 하루가 걸리는 컨텐츠는 정말 하루가 걸려야 한다.
  * ex) 건설 등
* TIMESTAMP는 표준이다.

~~~C++
int a = rand();

// ASM
imul	eax, dword ptr[seedAsm], 343FDh
add		eax, 269EC3h
mov		dword ptr[seedAsm], eax
shr		eax, 10h
and		eax, 7FFFh

// C로 구현
int a = seed * 0x343FD;
a += 0x269EC3
seed = a;
a = (a >> 16) & 0x7FFF
~~~
* 프로그래머에게 좋은 난수 = 균등한 난수
* 암호학적으로 안전하지 않다. -> 예측이 가능하다.

조금 더 안전하고 정교한 랜덤 함수
* \<stdlib.h>의 rand_s
* 아예 커널 모드로 전환해서 실행
* 분석이 불가능하다.
* 정교한 알고리즘은 무겁다.