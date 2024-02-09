#pragma once

struct stTimer
{
	// 시작 시간 계산용
	unsigned __int64 m_uliStartTime;
	unsigned __int64 m_uliCurTime;
};


extern stTimer g_stTimer;

void TimerInit(void);

void TimerUpdate(void);

int TimerGetTimeDiff(unsigned __int64 prevTime);

bool TimerCalCoolTime(unsigned __int64 &prevTime, int coolTime);

