#include "pch.h"
#include "TimeManager.h"

void TimerInit(void)
{
    g_stTimer.m_uliStartTime = GetTickCount64();
    TimerUpdate();
}

void TimerUpdate(void)
{
    // ���� �ð��� ���� 0���� �󸶳� �ð��� �귶������ ����
    g_stTimer.m_uliCurTime = (int)(GetTickCount64() - g_stTimer.m_uliStartTime);
}

// ���� �ð��� ���� �󸶳� �����°�
int TimerGetTimeDiff(unsigned int prevTime)
{
    return (int)(g_stTimer.m_uliCurTime - prevTime);
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
