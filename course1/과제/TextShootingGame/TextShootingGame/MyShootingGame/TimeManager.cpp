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

// ���ڷ� ���� ��Ÿ������ ���� ��� �������� ����
// int coolTime : ms ����
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime)
{
    if (TimerGetTimeDiff(prevTime) < coolTime)
    {
        return false;
    }
   
    prevTime = g_stTimer.m_uliCurTime; // ���� �ð����� ����
    return true;
}
