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

// ���� �ð��� ���� �󸶳� �����°�
int TimerGetTimeDiff(unsigned __int64 prevTime)
{
    return (int)(g_stTimer.m_uliCurTime - prevTime);
}

// ���ڷ� ���� ��Ÿ������ ���� ��� �������� ����
// int coolTime : ms ����
bool TimerCalCoolTime(unsigned __int64 &prevTime, int coolTime)
{
    if (TimerGetTimeDiff(prevTime) < coolTime)
    {
        return false;
    }
   
    prevTime = g_stTimer.m_uliCurTime; // ���� �ð����� ����
    return true;
}
