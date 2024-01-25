#pragma once
enum class RESULT_CODE
{
	GREAT = 0,
	GOOD,
	NOGOOD,
	BAD,
	FAIL,
	NONE,
	END = 5,
};

struct stTime;

class CGameLogic
{
private:
	CGameLogic() = default;
	~CGameLogic();

public:
	static CGameLogic *GetInstance();
	void Init();
	void Update();
	inline RESULT_CODE *GetResultArr() { return m_ArrTimingResult; }

private:
	void GameLogic();

private:
	RESULT_CODE		*m_ArrTimingResult;
	int				m_TimingArrSize;
	int 			m_curIndex;
};

