#pragma once

struct stTimer
{
	unsigned __int64 m_uliStartTime;
	unsigned __int64 m_uliCurTime;
};


extern stTimer g_stTimer;

void TimerInit(void);

void TimerUpdate(void);

int TimerGetTimeDiff(unsigned __int64 prevTime);

bool TimerCalCoolTime(unsigned __int64 prevTime, int coolTime);

