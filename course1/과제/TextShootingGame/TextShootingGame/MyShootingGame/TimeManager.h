#pragma once

struct stTimer
{
	// ���� �ð� ����
	unsigned __int64 m_uliStartTime;
	int m_uliCurTime;
};


extern stTimer g_stTimer;

void TimerInit(void);

void TimerUpdate(void);

int TimerGetTimeDiff(int prevTime);

bool TimerCalCoolTime(int &prevTime, int coolTime);

