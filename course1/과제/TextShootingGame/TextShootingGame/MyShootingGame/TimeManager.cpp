#include "pch.h"
#include "TimeManager.h"


stTimer g_stTimer;

void TimerInit(void)
{
    g_stTimer.m_uliStartTime = GetTickCount64();
    g_stTimer.m_uliCurTime = GetTickCount64();
}

void TimerUpdate(void)
{
    g_stTimer.m_uliCurTime = GetTickCount64();
}

// 이전 시간과 지금 얼마나 지났는가
int TimerGetTimeDiff(unsigned __int64 prevTime)
{
    return 0;
}

// 인자로 받은 쿨타임으로 지금 사용 가능한지 여부
bool TimerCalCoolTime(unsigned __int64 prevTime, int coolTime)
{
    return false;
}
