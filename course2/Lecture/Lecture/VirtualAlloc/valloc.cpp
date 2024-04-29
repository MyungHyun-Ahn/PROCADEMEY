#include <stdio.h>
#include <windows.h>

int main()
{
	// 첫 번째 매개변수 NULL - 알아서
	// 절대 주소를 넣으면 그냥 할당 - 이미 COMMIT 상태라도 COMMIT - 위험한 행동
	// 이미 절대 주소가 COMMIT 상태여야만 성공
	char *p = (char *)VirtualAlloc((VOID *)0x00a00000, 4096 * 4, MEM_COMMIT, PAGE_READWRITE);
	// 해당 크기만큼 RESERVE
	// MEM_COMMIT 인자를 넣고 첫 번째 매개변수가 NULL이라면 바로 COMMIT


	// 64KB 가 할당 단위 나머지 56KB는 Free 상태로 남는다.
	char *p2 = (char *)VirtualAlloc(NULL, 4096 * 2, MEM_COMMIT, PAGE_READWRITE);
	// 확장이 불가는 - 애초에 Reserve 를 할당하려면 64KB로 시작 주소가 떨어져야 한다.
	// 그냥 불가능
	// 예약 크기 변경 또한 실패

	int bResult = VirtualFree(p2 + 4096, 4096, MEM_DECOMMIT);
	// DECOMMIT을 하면 RESERVE로 되돌림


	// 페이지 속성 변경
	// VirtualProtect();

}