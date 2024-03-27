#pragma once

typedef void (*fixedUpdate)(void);

class TimeManager
{
	SINGLE(TimeManager)

public:
	// 타이머 초기화
	void Init(void);

	// 타이머 해제
	void Release();

	// 타이머 갱신
	void Update();

	// 쿨타임 판정
	bool CalCoolTime(unsigned int &prevTime, int coolTime);

	// fixedUpdate
	inline void FixedUpdate(fixedUpdate update)
	{
		// 프레임이 밀린 것
		m_iCurTime = timeGetTime();
		// 밀린 만큼
		while (m_iCurTime - m_iPrevTimeFrame >= (1000 / dfFPS))
		{
			m_iPrevTimeFrame += 1000 / dfFPS;
			m_iUpdateFps++;
			update();
		}

		// 일반 Update 취급
		m_iUpdateFps++;
		update();
	}

	// 랜더 까지 끝난 시간 체크하여 Sleep
	inline void RenderCheck()
	{
		m_iRenderFps++;
		m_iCurTime = timeGetTime();
		if (m_iCurTime - m_iPrevTimeFrame < 1000 / dfFPS)
			Sleep(1000 / dfFPS - (m_iCurTime - m_iPrevTimeFrame));
		m_iPrevTimeFrame += 1000 / dfFPS;
	}

	// 시간 차이 계산
	inline int GetTimeDiff(unsigned int prevTime)
	{
		return (int)(m_uliCurTime - prevTime);
	}

private:
	// 시간 동기화 전용
	int m_iPrevTimeFrame;
	int m_iPrevTimeSec;
	int m_iCurTime;

	int m_iUpdateFps;
	int m_iRenderFps;

	double m_dFixedDeltaTime = 0;
	// 시작 시간 계산용 - 100ms 이상 쿨타임 전용
	unsigned __int64 m_uliStartTime;
	unsigned int m_uliCurTime;
};

extern TimeManager *g_TimeMgr;