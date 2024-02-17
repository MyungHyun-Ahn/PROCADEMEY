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
void TimerUpdate(void);

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

