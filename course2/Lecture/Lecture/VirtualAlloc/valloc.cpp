#include <stdio.h>
#include <windows.h>

int main()
{
	// ù ��° �Ű����� NULL - �˾Ƽ�
	// ���� �ּҸ� ������ �׳� �Ҵ� - �̹� COMMIT ���¶� COMMIT - ������ �ൿ
	// �̹� ���� �ּҰ� COMMIT ���¿��߸� ����
	char *p = (char *)VirtualAlloc((VOID *)0x00a00000, 4096 * 4, MEM_COMMIT, PAGE_READWRITE);
	// �ش� ũ�⸸ŭ RESERVE
	// MEM_COMMIT ���ڸ� �ְ� ù ��° �Ű������� NULL�̶�� �ٷ� COMMIT


	// 64KB �� �Ҵ� ���� ������ 56KB�� Free ���·� ���´�.
	char *p2 = (char *)VirtualAlloc(NULL, 4096 * 2, MEM_COMMIT, PAGE_READWRITE);
	// Ȯ���� �Ұ��� - ���ʿ� Reserve �� �Ҵ��Ϸ��� 64KB�� ���� �ּҰ� �������� �Ѵ�.
	// �׳� �Ұ���
	// ���� ũ�� ���� ���� ����

	int bResult = VirtualFree(p2 + 4096, 4096, MEM_DECOMMIT);
	// DECOMMIT�� �ϸ� RESERVE�� �ǵ���


	// ������ �Ӽ� ����
	// VirtualProtect();

}