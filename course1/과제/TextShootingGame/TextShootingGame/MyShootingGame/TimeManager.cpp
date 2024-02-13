#include "pch.h"
#include "TimeManager.h"

void TimerInit(void)
{
    g_stTimer.m_uliStartTime = GetTickCount64();
    TimerUpdate();
}

void TimerUpdate(void)
{
    // 시작 시간을 빼서 0부터 얼마나 시간이 흘렀는지를 측정
    g_stTimer.m_uliCurTime = (int)(GetTickCount64() - g_stTimer.m_uliStartTime);
}

// 이전 시간과 지금 얼마나 지났는가
int TimerGetTimeDiff(unsigned int prevTime)
{
    return (int)(g_stTimer.m_uliCurTime - prevTime);
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
