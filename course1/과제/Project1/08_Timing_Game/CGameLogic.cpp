#include "pch.h"

CGameLogic::~CGameLogic()
{
	if (m_ArrTimingResult != nullptr)
	{
		free(m_ArrTimingResult);
	}
}

CGameLogic *CGameLogic::GetInstance()
{
	static CGameLogic gameLogic;
	return &gameLogic;
}

void CGameLogic::Init()
{
	m_TimingArrSize = sizeof(g_Timing) / sizeof(int);
	m_curIndex = 0;

	m_ArrTimingResult = (RESULT_CODE *)malloc(m_TimingArrSize * sizeof(int));
	if (m_ArrTimingResult == nullptr)
	{
		_tprintf(TEXT("malloc 실패!"));
		return;
	}

	for (size_t i = 0; i < m_TimingArrSize; i++)
	{
		*(m_ArrTimingResult + i) = RESULT_CODE::NONE;
	}
}

void CGameLogic::Update()
{
	GameLogic();
}



void CGameLogic::GameLogic()
{
	if (m_curIndex >= m_TimingArrSize)
	{
		// _tprintf(TEXT("게임 종료"));
		return;
	}

	ULONGLONG curTime = CTimeManager::GetInstance()->GetTick();
	INT judgeTime = g_Timing[m_curIndex];
	if (judgeTime * (MS_PER_SEC) + MS_PER_SEC < curTime) // 1초 이상 지난 경우
	{
		m_ArrTimingResult[m_curIndex++] = RESULT_CODE::FAIL;
	}

	KEY_FLAGS keyFlags = CKeyManager::GetInstance()->GetFlag();
	if (keyFlags == KEY_FLAGS::KEY_UP)
		return;

	LONG lRes = labs(judgeTime * MS_PER_SEC - curTime);

	if (lRes < (MS_PER_SEC / 4)) // 오차가 가장 작은 것
	{
		m_ArrTimingResult[m_curIndex++] = RESULT_CODE::GREAT;
	}
	else if (lRes < (MS_PER_SEC / 4) * 2) // 2
	{
		m_ArrTimingResult[m_curIndex++] = RESULT_CODE::GOOD;
	}
	else if (lRes < (MS_PER_SEC / 4) * 3)
	{
		m_ArrTimingResult[m_curIndex++] = RESULT_CODE::NOGOOD;
	}
	else
	{
		m_ArrTimingResult[m_curIndex++] = RESULT_CODE::BAD;
	}
}