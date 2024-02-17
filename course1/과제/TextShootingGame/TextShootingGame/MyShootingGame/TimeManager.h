#pragma once

struct stTimer
{
	// �ð� ����ȭ ����
	int m_iPrevTimeFrame;
	int m_iPrevTimeSec;
	int m_iCurTime;

	int m_iUpdateFps;
	int m_iRenderFps;

	double m_dFixedDeltaTime = 0;
	// ���� �ð� ���� - 100ms �̻� ��Ÿ�� ����
	unsigned __int64 m_uliStartTime;
	unsigned int m_uliCurTime;
};

// Ÿ�̸� �ʱ�ȭ
void TimerInit(void);

// Ÿ�̸� ����
void TimerRelease(void);

// Ÿ�̸� ����
void TimerUpdate(void);

// fixedUpdate
inline void TimerFixedUpdate(fixedUpdate fix)
{
	// �������� �и� ��
	g_stTimer.m_iCurTime = timeGetTime();
	// �и� ��ŭ
	while (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeFrame >= (1000 / dfFPS))
	{
		g_stTimer.m_iPrevTimeFrame += 1000 / dfFPS;
		g_stTimer.m_iUpdateFps++;
		fix();
	}

	// �Ϲ� Update ���
	g_stTimer.m_iUpdateFps++;
	fix();
}

// ���� ���� ���� �ð� üũ�Ͽ� Sleep
inline void TimerRenderCheck()
{
	g_stTimer.m_iRenderFps++;
	g_stTimer.m_iCurTime = timeGetTime();
	if (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeFrame < 1000 / dfFPS)
		Sleep(1000 / dfFPS - (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeFrame));
	g_stTimer.m_iPrevTimeFrame += 1000 / dfFPS;
}

// �ð� ���� ���
inline int TimerGetTimeDiff(unsigned int prevTime)
{
	return (int)(g_stTimer.m_uliCurTime - prevTime);
}


// ��Ÿ�� ����
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime);

