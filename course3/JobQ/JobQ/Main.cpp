#include "pch.h"
#include "RingBuffer.h"

#define dfJOB_ADD	0
LONG addCnt = 0;

#define dfJOB_DEL	1
LONG delCnt = 0;

#define dfJOB_SORT	2
LONG sortCnt = 0;

#define dfJOB_FIND	3
LONG findCnt = 0;

#define dfJOB_PRINT	4	// << ��� or ���� / �б⸸ �ϴ� ���� �ൿ
LONG printCnt = 0;

#define dfJOB_QUIT	5


const WCHAR *randStr[5] = {
	L"AAAAA",
	L"BBBBB",
	L"CCCCC",
	L"DDDDD",
	L"EEEEE"
};

struct stMsgHead
{
	SHORT shType;
	SHORT shPayloadLen;
};

RingBuffer g_msgQ;
std::list<std::wstring> g_List;

SRWLOCK g_listSRW;

HANDLE g_hEvent;

bool compare(const std::wstring &first, const std::wstring &second)
{
	unsigned int i = 0;
	while ((i < first.length()) && (i < second.length()))
	{
		if (std::towlower(first[i]) < std::towlower(second[i])) return true;
		else if (std::towlower(first[i]) > std::towlower(second[i])) return false;
		++i;
	}
	return (first.length() < second.length());
}

unsigned WorkerThreadProc(LPVOID lpParam)
{
	BOOL running = TRUE;
	while (running)
	{
		srand(GetCurrentThreadId());

		WaitForSingleObject(g_hEvent, INFINITE);

		// Event�� ���� ����

		// �� �̻� ó���Ұ� ���� ������ �ݺ�
		while (TRUE)
		{
			g_msgQ.WLock();
			if (g_msgQ.GetUseSize() < sizeof(stMsgHead))
			{
				g_msgQ.WUnLock();
				break;
			}

			stMsgHead header;
			int ret = g_msgQ.Peek((char *)&header, sizeof(stMsgHead));

			if (g_msgQ.GetUseSize() < header.shPayloadLen)
			{
				g_msgQ.WUnLock();
				break;
			}

			// Dequeue
			g_msgQ.MoveFront(ret);

			if (header.shPayloadLen == 0)
				g_msgQ.WUnLock();

			switch (header.shType)
			{

			case dfJOB_ADD:
			{
				WCHAR *pWCH = new WCHAR[header.shPayloadLen + 1];
				g_msgQ.Dequeue((char *)pWCH, header.shPayloadLen);
				g_msgQ.WUnLock();

				pWCH[header.shPayloadLen / sizeof(WCHAR)] = L'\0';
				std::wstring wstr = pWCH;

				AcquireSRWLockExclusive(&g_listSRW);
				g_List.push_back(wstr);
				ReleaseSRWLockExclusive(&g_listSRW);

				InterlockedIncrement(&addCnt);

				delete [] pWCH;
			}
			break;
			case dfJOB_DEL:
			{
				AcquireSRWLockExclusive(&g_listSRW);
				if (g_List.size() == 0)
				{
					ReleaseSRWLockExclusive(&g_listSRW);
					break;
				}

				int n = rand() % g_List.size();

				auto it = g_List.begin();
				std::advance(it, n);
				g_List.erase(it);
				ReleaseSRWLockExclusive(&g_listSRW);

				InterlockedIncrement(&delCnt);

			}
			break;
			case dfJOB_SORT:
			{
				AcquireSRWLockExclusive(&g_listSRW);
				g_List.sort(compare);
				ReleaseSRWLockExclusive(&g_listSRW);

				InterlockedIncrement(&sortCnt);
			}
			break;
			case dfJOB_FIND:
			{
				AcquireSRWLockExclusive(&g_listSRW);
				if (g_List.size() == 0)
				{
					ReleaseSRWLockExclusive(&g_listSRW);

					InterlockedIncrement(&findCnt);
					break;
				}

				int n = rand() % g_List.size();
				
				auto it = g_List.begin();
				std::advance(it, n);

				ReleaseSRWLockExclusive(&g_listSRW);

				InterlockedIncrement(&findCnt);
			}
			break;
			case dfJOB_PRINT:

			{
				AcquireSRWLockExclusive(&g_listSRW);
				if (g_List.size() == 0)
				{
					ReleaseSRWLockExclusive(&g_listSRW);

					InterlockedIncrement(&printCnt);
					break;
				}

				// 1�� ã��
				int n = rand() % g_List.size();
				auto it = g_List.begin();
				std::advance(it, n);

				wprintf(L"%s\n", *it);
				// std::wcout << *it << std::endl;

				ReleaseSRWLockExclusive(&g_listSRW);

				InterlockedIncrement(&printCnt);
			}
			break;
			case dfJOB_QUIT:
				running = FALSE;
				break;
			}

		}
	}

	return 0;
}

unsigned MonitoringThreadProc(LPVOID lpParam)
{
	BOOL running = TRUE;

	int prevTime = timeGetTime();
	while (running)
	{

		int now = timeGetTime();
		if (now - prevTime > 1000)
		{
			prevTime = now;
			g_msgQ.RLock();
			// �޽��� ��ť�� ������(��뷮) ���
			std::wcout << L"msgQ used size : " << g_msgQ.GetUseSize() << std::endl;
			g_msgQ.RUnLock();
			// �ʴ� �޽��� �� ��ü ó�� �� (TPS) + ������ �� ó�� ��
			std::wcout << L"TPS : " << addCnt + delCnt + sortCnt + findCnt + printCnt << std::endl;

			// �ʴ� Ÿ�Ժ� �� ó�� �� (�޽��� �� TPS)
			std::wcout << L"ADD TPS : " << addCnt << std::endl;
			std::wcout << L"DEL TPS : " << delCnt << std::endl;
			std::wcout << L"SORT TPS : " << sortCnt << std::endl;
			std::wcout << L"FIND TPS : " << findCnt << std::endl;
			std::wcout << L"PRINT TPS : " << printCnt << std::endl;

			InterlockedExchange(&addCnt, 0);
			InterlockedExchange(&delCnt, 0);
			InterlockedExchange(&sortCnt, 0);
			InterlockedExchange(&findCnt, 0);
			InterlockedExchange(&printCnt, 0);
		}
	}

	return 0;
}

int main()
{
	srand(time(nullptr));

	g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hEvent == 0)
		return 1;

	InitializeSRWLock(&g_listSRW);

	HANDLE arrWorkerTh[3];
	
	for (int i = 0; i < 3; i++)
	{
		arrWorkerTh[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThreadProc, nullptr, CREATE_SUSPENDED, nullptr);
	}

	HANDLE hMonitoringTh = (HANDLE)_beginthreadex(nullptr, 0, MonitoringThreadProc, nullptr, CREATE_SUSPENDED, nullptr);

	for (int i = 0; i < 3; i++)
	{
		ResumeThread(arrWorkerTh[i]);
	}

	ResumeThread(hMonitoringTh);

	BOOL running = TRUE;
	while (running)
	{
		// �޽��� ����
		// �� Ÿ���� �����ϰ� ����
		// �⿡ ���� �߰����� ������(���ڿ�)�� �����ϰ� ����
		
		stMsgHead header;
		header.shType = rand() % 5;
		
		switch (header.shType)
		{
		
		case dfJOB_ADD:
		{
			SHORT n = rand() % 5;
			header.shPayloadLen = wcslen(randStr[n]) * sizeof(WCHAR);

			g_msgQ.WLock();
			g_msgQ.Enqueue((char *)&header, sizeof(stMsgHead));
			g_msgQ.Enqueue((char *)randStr[n], header.shPayloadLen);
			g_msgQ.WUnLock();

			SetEvent(g_hEvent);
		}
			break;
		case dfJOB_DEL:
		case dfJOB_SORT:
		case dfJOB_FIND:
		case dfJOB_PRINT:

		{
			header.shPayloadLen = 0;
			g_msgQ.WLock();
			g_msgQ.Enqueue((char *)&header, sizeof(stMsgHead));
			g_msgQ.WUnLock();
				
			SetEvent(g_hEvent);
		}
			break;
		case dfJOB_QUIT:
			running = FALSE;
			break;
		}

		Sleep(5);
	}

}