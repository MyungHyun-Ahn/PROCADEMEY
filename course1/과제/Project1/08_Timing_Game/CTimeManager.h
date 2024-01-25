#pragma once
struct stTime
{
	ULONGLONG m_ullSecond;
	ULONGLONG m_ullMillsecond;
};

class CTimeManager
{
private:
	CTimeManager() = default;
	~CTimeManager() = default;

public:
	static CTimeManager			*GetInstance();
	void						Init();
	void						Update();
	inline stTime				&GetTime() { return m_stTime; }
	inline ULONGLONG			GetTick() { return GetTickCount64() - m_ullStartTime; }

private:
	void						calCurTime();

private:
	ULONGLONG		m_ullStartTime;
	stTime			m_stTime;
};

