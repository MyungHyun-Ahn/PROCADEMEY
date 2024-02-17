#include "pch.h"
#include "TimeManager.h"
#include "SceneManager.h"

void TimerInit(void)
{
    timeBeginPeriod(dfTIME_PERIOD);
    g_stTimer.m_iPrevTimeFrame = timeGetTime();
    g_stTimer.m_iPrevTimeSec = g_stTimer.m_iPrevTimeFrame;
    g_stTimer.m_iCurTime = g_stTimer.m_iCurTime;
    
	g_stTimer.m_iUpdateFps = 0;
	g_stTimer.m_iRenderFps = 0;

    g_stTimer.m_uliStartTime = GetTickCount64();
    TimerUpdate();
}

void TimerRelease(void)
{
    timeEndPeriod(dfTIME_PERIOD);
}

// 인자로 받은 쿨타임으로 지금 사용 가능한지 여부
// int coolTime : ms 단위
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime)
{
    if (TimerGetTimeDiff(prevTime) < coolTime)
    {
        return false;
    }
   
    prevTime = g_stTimer.m_uliCurTime; // 현재 시간으로 갱신
    return true;
}
