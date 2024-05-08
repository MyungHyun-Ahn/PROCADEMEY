#include <stdio.h>
#include <conio.h>
#include <windows.h>

constexpr int PAGE_SIZE = 4 * 1024;
constexpr int STACK_SIZE = 1024 * 1024;

int main()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	MEMORY_BASIC_INFORMATION mbi;
	/*
	AllocationBase : ù �Ҵ� ���� ��ġ
		* ���ÿ��� ����ϸ� ������ ������
	BaseAddress : �ش� �Ӽ��� ���� ��ġ
	RegionSize : ���� �Ӽ��� ũ��
	
	*/
	VirtualQuery(&si, &mbi, sizeof(mbi));
	UINT_PTR pStackEnd = (UINT_PTR)mbi.AllocationBase + STACK_SIZE;
	BYTE *pGuard = nullptr;

	while (true)
	{
		UINT_PTR pPosition = (UINT_PTR)mbi.AllocationBase;

		while (pPosition < pStackEnd)
		{
			SIZE_T size = VirtualQuery((LPCVOID)pPosition, &mbi, sizeof(mbi));
			if (!size) __debugbreak();

			printf("addr : %x, region size : %d KBytes\n", pPosition, mbi.RegionSize / 1024);
			printf("Page Protection : ");
			if (mbi.Protect & PAGE_READWRITE)
				printf("PAGE_READWRITE ");
			if (mbi.Protect & PAGE_GUARD)
				printf("PAGE_GUARD ");
			printf("\n");

			printf("Virtual Memory Allocation State : ");
			switch (mbi.State)
			{
			case MEM_COMMIT:
				printf("MEM_COMMIT ");
				break;
			case MEM_RESERVE:
				printf("MEM_RESERVE");
				break;
			default:
				printf("MEM_FREE");
				break;
			}

			printf("\n\n");
			pPosition += mbi.RegionSize;
		}
		printf("--------------------------------------\n");

		// ���� ������ ���ϱ�
		pGuard = (BYTE *)(pPosition - mbi.RegionSize - PAGE_SIZE);
		__try
		{
			// ���� ������ ���
			*pGuard = 1;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			// StackOverFlow
			if (GetExceptionCode() == EXCEPTION_STACK_OVERFLOW)
			{
				printf("Stack OverFlow Occur");
				return 0;
			}
			else
			{
				__debugbreak();
			}
		}

		_getch();
	}

	return 0;
}