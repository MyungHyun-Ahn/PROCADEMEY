#pragma once

struct stTimer
{
	// ���� �ð� ����
	unsigned __int64 m_uliStartTime;
	unsigned int m_uliCurTime;
};

// Ÿ�̸� �ʱ�ȭ
void TimerInit(void);

// Ÿ�̸� ����
void TimerUpdate(void);

// �ð� ���� ���
int TimerGetTimeDiff(unsigned int prevTime);

// ��Ÿ�� ����
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime);

