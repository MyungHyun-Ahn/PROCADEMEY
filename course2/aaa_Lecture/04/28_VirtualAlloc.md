# VirtualAlloc

### SYSTEM_INFO 구조체
```C++
typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;
```
* 시스템 관련 정보를 얻을 수 있음
* dwAllocationGranularity : 할당 단위 64K
  * Reserve 시 영향
  * 메모리 관리 자체를 64K 단위로 진행
  * 한 번 reserve를 하고 commit을 했으면 다시 확장 불가능

### VirtualAlloc
```C++
VirtualAlloc(포인터, 크기, 할당 타입, 권한);
```
* VirtualAlloc을 한 영역은 0으로 밀리는 것을 보장
* 이미 커밋된 영역을 예약 상태로 바꿀 수 없다

포인터
* 할당 과정의 시작 주소
* 64KB 경계에 알아서 맞춤 - 내림
* NULL인 경우 시스템이 알아서 결정
  * 보통은 NULL로 사용

할당 타입
* commit
* reserve
  * 포인터에 NULL이 들어오면 MEM_COMMIT으로 바로 진행 가능
  * 하지만 NULL이 아니라면 먼저 예약을 수행

### 페이지 상태 변화 가능 여부
* COMMIT -> RESERVE : 불가능
* DECOMMIT -> RESERVE : 가능

### VirtualFree
VirtualFree
* DeCommit
* Release(크기 지정 불가)
  * 크기 0으로 지정
  * 64K 통으로 날림

Commit -> Reserve
* VirtualFree 함수로 디커밋

### 페이지 단위 할당
번잡해서 잘 안씀
* 힙이 편해서 힙 사용

### 언제 씀?
페이지 속성을 바꾸고 싶을 때
* PAGE_READONLY
* PAGE_READWRITE

VirtualAlloc으로 Commit -> Commit 가능
* 속성만 바꿀 수 있음
* 혹은 VirtualProtect 사용

### 브레이크 포인트 작동 원리
* 소프트웨어 인터럽트를 발생시키는 것
* int 3 (어셈블리 명령어 : 3번 인터럽트 발생 명령)
  * 바이너리 코드 : CC
* 브레이크 포인트는 디버거가 예외처리를 해줌
  * __debugbreak()와 동일

VS에서 찍는 중단점
* 디버거가 예외를 가로챔
* 그 지점을 찾아서 중단
* 런타임에 코드를 삽입
  * 원래 있던 코드에 CC를 넣고, 중단점을 걸고, 다시 원래의 코드로 되돌림
* 런타임에 우리가 CC를 넣을 수 있다면 똑같은 효과

### 페이지 속성
내가 할당받은 메모리의 앞뒤를 건드렸을 때 언제 알아차릴 수 있는가?
* 일반적으로 해제할 때 알게됨

우리가 접근할 때 바로 알 수 있도록 하려면?
* 할당 받은 부분의 뒤 부분을 속성을 접근 불가 속성으로 설정

### 스택의 작동
실제 스택은 어떻게 작동하는가?

스택 Default 크기 1MB - RESERVE
* 처음 스택이 할당되면 4KB 할당
* 1MB의 맨 위 페이지를 접근하려하면 스택오버플로우
* 커밋 페이지 위쪽에 Guard 페이지를 둔다
  * x86 2개
  * x64 3개
* 가드 페이지는 커밋 상태

만약 지역변수가 너무 크다면
* 컴파일러가 개입 - 일부로 가드 페이지를 접근해 확장
* _chkstk 자동으로 호출

### VirtualQuery
메모리의 상태와 속성을 확인하는 함수

MEMORY_BASIC_INFORMATION 구조체
* AllocationAddress : 그 페이지를 할당할 때 64K 덩어리 할당 시작주소
* BaseAddress : 같은 속성/상태인 페이지의 시작 주소
* regionSize : 같은 속성/상태의 BaseAddress 부터 달라지는 위치까지의 사이즈

pStack + RegionSIze = 다음 속성의 페이지


### 게임 서버에서의 핵 방지
stateful 서버에서는 클라에서 메모리 변조를 해도 막을 필요가 없다.
* 막을 필요가 없다.
* 실질적인 연산을 서버가 수행하기 때문
* 서버에서 검증 - 서버 데이터 기준으로 연산
* 클라의 데이터를 믿으면 안된다.

해커가 조작을 할 때 어떻게 할까?
1. 조작을 원하는 값을 찾는다. - 메모리 서치
2. 모든 메모리에서 찾기 수행
3. 그 값을 변경 하고 다시 수행
4. 마지막에 남은 값을 수정

### 코드로 구현
ProcessID를 통해 핸들을 얻는다. OpenProcess()

VirtualQuery로 private 메모리만 검사
  * data, heap, stack, VirtualAlloc 영역
GetSystemInfo -> 최대, 최소 유저 영역 범위 가져오기

VirtualQueryEx -> 프로세스의 핸들을 받는다.
  * MEM_PRIVATE만 찾는다. + MEM_COMMIT

ReadProcessMemory - 메모리 읽기
* ReadProcessError -> 가드 설정 영역
  * 안읽힌다. -> PROTECT 확인

WriteProcessMemory - 값 변경

### 클라이언트에서 핵 방지
1. 체크섬
2. XOR로 수치 꼬기
3. 변화 감지하기