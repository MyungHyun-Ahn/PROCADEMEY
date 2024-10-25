#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <process.h>
#include <share.h>
#include "Logger.h"

#define LOOP_COUNT 100000000

// Log �ɼ�
#define LOG_MEMORY
// #define LOG_PRINT
#define LOG_FILE

#define LOAD_BARRIER

int g_Num = 0;


CHAR g_Flags[2];
CHAR g_Turn = 0;

LONG aThreadCnt = 0;
LONG g_Bool = FALSE;
LONG g_Comparand = TRUE;

struct DebugInfo
{
	CHAR isUnLock;
	CHAR threadNum;
	CHAR flag;
	CHAR loadFlag;
	CHAR turn;
	CHAR loadTurn;
};

LONG debugInfoIndex = -1;
DebugInfo debugInfo[LOOP_COUNT];

unsigned ThreadFunc1(LPVOID lpParam)
{
	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		// �ε� ������ flag�� turn�� ����
		CHAR flag;
		CHAR turn;

		// Lock
		{
			// store
			g_Flags[0] = TRUE;
			g_Turn = 0;

#ifdef LOAD_BARRIER
			// Load�� Store�� ������ �� ����
			// �ٵ� Load������ ������ �� ����
			// ������ �����ִ� Store�� Load ������ �Ʒ��� �ε� �ڵ尡 ������ �� ������
			CHAR loadBarrierFlag = g_Flags[0];
			CHAR loadBarrierTurn = g_Turn;
#endif

			while (1)
			{
				// load
				flag = g_Flags[1];
				turn = g_Turn;

				if (flag == FALSE)
					break;

				if (turn != 0)
					break;
			}
		}

		// Interlocked ���� - �׷��� ������ ������ ���� Ȯ���� �پ��
		g_Num++;

		// thread count�� �ø��� ���� �ٵ� 2�� 2�� ��
		if (InterlockedIncrement(&aThreadCnt) == 2)
		{
			// ���� ������ �ε� ������ ������ ������
			INT index = InterlockedIncrement(&debugInfoIndex);
#ifdef LOG_MEMORY
			debugInfo[index] = { 0, 1, g_Flags[1], flag, g_Turn, turn };
#endif
			// I/O�� �ɾ������ ��Ȳ�� �� ��Ÿ����?
			// �ߴµ� �� ���� ����?
#ifdef LOG_PRINT
			wprintf(L"%d Lock \t ThreadFunc1 : Flag : %d, Turn : %d, loadFlag : %d loadTurn : %d\n", index, g_Flags[1], g_Turn, flag, turn);
#endif
			// __debugbreak();
		}

		// UnLock
		{
			// ����µ� 1�̸� �ٸ� �����嵵 �ִ� ��
			if (InterlockedDecrement(&aThreadCnt) == 1)
			{
				INT index = InterlockedIncrement(&debugInfoIndex);
#ifdef LOG_MEMORY
				debugInfo[index] = { 1, 1, g_Flags[1], flag, g_Turn, turn };
#endif
#ifdef LOG_PRINT
				wprintf(L"%d UnLock \t ThreadFunc1 : Flag : %d, Turn : %d, loadFlag : %d loadTurn : %d\n", index, g_Flags[1], g_Turn, flag, turn);
#endif
			
				// __debugbreak();
			}
			g_Flags[0] = FALSE;
		}

	}

	return 0;
}

unsigned ThreadFunc2(LPVOID lpParam)
{
	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		CHAR flag;
		CHAR turn;

		// Lock
		{
			// store
			g_Flags[1] = TRUE;
			g_Turn = 1;

#ifdef LOAD_BARRIER
			CHAR loadBarrierFlag = g_Flags[1];
			CHAR loadBarrierTurn = g_Turn;
#endif

			while (1)
			{
				// load
				flag =  g_Flags[0];
				turn = g_Turn;

				if (flag == FALSE)
					break;

				if (turn != 1)
					break;
			}
		}

		// Interlocked ���� - �׷��� ������ ������ ���� Ȯ���� �پ��
		g_Num++;

		// thread count�� �ø��� ���� �ٵ� 2�� 2�� ��
		if (InterlockedIncrement(&aThreadCnt) == 2)
		{
			INT index = InterlockedIncrement(&debugInfoIndex);
			// ���� ������ �ε� ������ ������ ������
#ifdef LOG_MEMORY
			debugInfo[index] = { 0, 2, g_Flags[0], flag, g_Turn, turn };
#endif

#ifdef LOG_PRINT
			wprintf(L"%d Lock \t ThreadFunc2 : Flag : %d, Turn : %d, loadFlag : %d loadTurn : %d\n", index, g_Flags[0], g_Turn, flag, turn);
#endif
		
			// __debugbreak();
		}

		// UnLock
		{
			// ����µ� 1�̸� �ٸ� �����嵵 �ִ� ��
			if (InterlockedDecrement(&aThreadCnt) == 1)
			{
				INT index = InterlockedIncrement(&debugInfoIndex);

#ifdef LOG_MEMORY
				debugInfo[index] = { 1, 2, g_Flags[0], flag, g_Turn, turn };
#endif

#ifdef LOG_PRINT
				wprintf(L"%d UnLock \t ThreadFunc2 : Flag : %d, Turn : %d, loadFlag : %d loadTurn : %d\n", index, g_Flags[0], g_Turn, flag, turn);
#endif
				// __debugbreak();
			}
			g_Flags[1] = FALSE;
		}

	}

	return 0;
}

int g_TestCount = 0;

void TestFunc()
{
	wprintf(L"Test %d : Start\n", g_TestCount);
#ifdef LOG_MEMORY
	g_Logger = Logger::GetInstance();
	g_Logger->SetDirectory(L"LOG_FILE");
	g_Logger->SetLogLevel(LOG_LEVEL::DEBUG);

	g_Logger->WriteLog(L"LOG", LOG_LEVEL::DEBUG, L"LOG_START");
#endif

	g_Flags[0] = FALSE;
	g_Flags[1] = FALSE;

	HANDLE arrThread[2] = { 0, 0 };

	arrThread[0] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc1, nullptr, CREATE_SUSPENDED, nullptr);
	if (arrThread[0] == 0)
	{
		return;
	}

	arrThread[1] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc2, nullptr, CREATE_SUSPENDED, nullptr);
	if (arrThread[1] == 0)
	{
		return;
	}

	ResumeThread(arrThread[0]);
	ResumeThread(arrThread[1]);

	WaitForMultipleObjects(2, arrThread, TRUE, INFINITE);

	const WCHAR* isLock = L"Lock";
	const WCHAR* isUnLock = L"UnLock";

#ifdef LOG_MEMORY
	for (int i = 0; i < debugInfoIndex; i++)
	{
		DebugInfo* info = &debugInfo[i];

		const WCHAR* strLock;
		if (info->isUnLock)
			strLock = isUnLock;
		else
			strLock = isLock;

		wprintf(L"%d %s \t ThreadFunc%d : Flag : %d Turn : %d, loadFlag : %d loadTurn : %d\n", i, strLock, info->threadNum, info->flag, info->turn, info->loadFlag, info->loadTurn);

		g_Logger->WriteLog(L"LOG", LOG_LEVEL::DEBUG, L"%d %s \t ThreadFunc%d : Flag : %d Turn : %d, loadFlag : %d loadTurn : %d", i, strLock, info->threadNum, info->flag, info->turn, info->loadFlag, info->loadTurn);
	}
	g_Logger->WriteLog(L"LOG", LOG_LEVEL::DEBUG, L"Expect : %d, Result : %d, Error : %d", LOOP_COUNT * 2, g_Num, LOOP_COUNT * 2 - g_Num);
	g_Logger->WriteLog(L"LOG", LOG_LEVEL::DEBUG, L"LOG_END\n\n\n");
#endif


	wprintf(L"NUM : %d\n", g_Num);
	wprintf(L"Test %d : END\n", g_TestCount);
	g_Num = 0;
	CloseHandle(arrThread[0]);
	CloseHandle(arrThread[1]);
}

int main()
{
	int testCnt;
	wprintf(L"test count : ");
	wscanf_s(L"%d", &testCnt);

	for (; g_TestCount < testCnt; g_TestCount++)
	{
		TestFunc();
	}

	return 0;
}
