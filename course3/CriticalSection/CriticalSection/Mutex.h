#pragma once

// #define RECURSION_MODE

class Mutex
{
public:
	Mutex() = default;
	Mutex(DWORD spinCount) : m_SpinCount(spinCount) {}

	void Lock()
	{
#ifdef RECURSION_MODE

		DWORD threadID = GetThreadId(GetCurrentThread());
		
		if (threadID == m_threadID)
		{
			m_RecursionCount++;
			return;
		}
		
#endif

		int count = 0;
		while (InterlockedExchange(&m_Key, TRUE) == TRUE)
		{
			// SpinLock
			if (count++ < m_SpinCount)
			{
				// YieldProcessor 한번으로는 충분한 시간을 벌어주지 못함
				for (int i = 0; i < 1024; ++i)
					YieldProcessor();
				continue;
			}

			WaitOnAddress(&m_Key, &m_Comparand, sizeof(LONG), INFINITE);
		}
#ifdef RECURSION_MODE
		m_RecursionCount++;
		m_threadID = threadID;
#endif
	}

	void UnLock()
	{
#ifdef RECURSION_MODE
		m_RecursionCount--;
		if (m_RecursionCount == 0)
		{
			m_threadID = NULL;
			m_Key = FALSE;
			WakeByAddressSingle(&m_Key);
		}

#else
		m_Key = FALSE;
		WakeByAddressSingle(&m_Key);
#endif
	}

private:
	LONG m_Key = FALSE;
	LONG m_Comparand = TRUE;
	DWORD m_RecursionCount = 0;
	DWORD m_SpinCount = 1000;
	DWORD m_threadID = NULL;
};

