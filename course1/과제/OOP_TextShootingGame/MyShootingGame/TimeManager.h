#pragma once

typedef void (*fixedUpdate)(void);

class TimeManager
{
	SINGLE(TimeManager)

public:
	// Ÿ�̸� �ʱ�ȭ
	void Init(void);

	// Ÿ�̸� ����
	void Release();

	// Ÿ�̸� ����
	void Update();

	// ��Ÿ�� ����
	bool CalCoolTime(unsigned int &prevTime, int coolTime);

	// fixedUpdate
	inline void FixedUpdate(fixedUpdate update)
	{
		// �������� �и� ��
		m_iCurTime = timeGetTime();
		// �и� ��ŭ
		while (m_iCurTime - m_iPrevTimeFrame >= (1000 / dfFPS))
		{
			m_iPrevTimeFrame += 1000 / dfFPS;
			m_iUpdateFps++;
			update();
		}

		// �Ϲ� Update ���
		m_iUpdateFps++;
		update();
	}

	// ���� ���� ���� �ð� üũ�Ͽ� Sleep
	inline void RenderCheck()
	{
		m_iRenderFps++;
		m_iCurTime = timeGetTime();
		if (m_iCurTime - m_iPrevTimeFrame < 1000 / dfFPS)
			Sleep(1000 / dfFPS - (m_iCurTime - m_iPrevTimeFrame));
		m_iPrevTimeFrame += 1000 / dfFPS;
	}

	// �ð� ���� ���
	inline int GetTimeDiff(unsigned int prevTime)
	{
		return (int)(m_uliCurTime - prevTime);
	}

private:
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

extern TimeManager *g_TimeMgr;