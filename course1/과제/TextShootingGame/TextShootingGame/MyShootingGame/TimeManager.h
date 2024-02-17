#pragma once

struct stTimer
{
	// 시간 동기화 전용
	int m_iPrevTimeFrame;
	int m_iPrevTimeSec;
	int m_iCurTime;

	int m_iUpdateFps;
	int m_iRenderFps;

	double m_dFixedDeltaTime = 0;
	// 시작 시간 계산용 - 100ms 이상 쿨타임 전용
	unsigned __int64 m_uliStartTime;
	unsigned int m_uliCurTime;
};

// 타이머 초기화
void TimerInit(void);

// 타이머 해제
void TimerRelease(void);

// 타이머 갱신
inline void TimerUpdate(void)
{
	// 시작 시간을 빼서 0부터 얼마나 시간이 흘렀는지를 측정
	g_stTimer.m_uliCurTime = (int)(GetTickCount64() - g_stTimer.m_uliStartTime);

	g_stTimer.m_iCurTime = timeGetTime();
	if (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeSec >= 1000)
	{
		char title[256];
		sprintf_s(title, "Text Shooting Game : Update : %d, Render : %d", g_stTimer.m_iUpdateFps, g_stTimer.m_iRenderFps);
		SetConsoleTitleA(title);
		g_stTimer.m_iPrevTimeSec += 1000;
		g_stTimer.m_iUpdateFps = 0;
		g_stTimer.m_iRenderFps = 0;
	}
}

// fixedUpdate
inline void TimerFixedUpdate(fixedUpdate fix)
{
	// 프레임이 밀린 것
	g_stTimer.m_iCurTime = timeGetTime();
	// 밀린 만큼
	while (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeFrame >= (1000 / dfFPS))
	{
		g_stTimer.m_iPrevTimeFrame += 1000 / dfFPS;
		g_stTimer.m_iUpdateFps++;
		fix();
	}

	// 일반 Update 취급
	g_stTimer.m_iUpdateFps++;
	fix();
}

// 랜더 까지 끝난 시간 체크하여 Sleep
inline void TimerRenderCheck()
{
	g_stTimer.m_iRenderFps++;
	g_stTimer.m_iCurTime = timeGetTime();
	if (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeFrame < 1000 / dfFPS)
		Sleep(1000 / dfFPS - (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeFrame));
	g_stTimer.m_iPrevTimeFrame += 1000 / dfFPS;
}

// 시간 차이 계산
inline int TimerGetTimeDiff(unsigned int prevTime)
{
	return (int)(g_stTimer.m_uliCurTime - prevTime);
}


// 쿨타임 판정
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime);

