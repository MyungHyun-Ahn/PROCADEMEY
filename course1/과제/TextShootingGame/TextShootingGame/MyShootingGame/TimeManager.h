#pragma once

struct stTimer
{
	// 시작 시간 계산용
	unsigned __int64 m_uliStartTime;
	unsigned int m_uliCurTime;
};

// 타이머 초기화
void TimerInit(void);

// 타이머 갱신
void TimerUpdate(void);

// 시간 차이 계산
int TimerGetTimeDiff(unsigned int prevTime);

// 쿨타임 판정
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime);

