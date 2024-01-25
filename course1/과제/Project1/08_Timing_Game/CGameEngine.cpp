#include "pch.h"

CGameEngine::~CGameEngine()
{

}

CGameEngine *CGameEngine::GetInstance()
{
	static CGameEngine gameEngine;
	return &gameEngine;
}

void CGameEngine::Start()
{
	Init();
	GameLogic();
}

void CGameEngine::Init()
{
	CTimeManager::GetInstance()->Init();
	CKeyManager::GetInstance()->Init();
	CGameLogic::GetInstance()->Init();
}

void CGameEngine::GameLogic()
{
	while (true)
	{
		Update();
		Render();
	}
}

void CGameEngine::Update()
{
	CTimeManager::GetInstance()->Update();
	CKeyManager::GetInstance()->Update();
	CGameLogic::GetInstance()->Update();
}

void CGameEngine::Render()
{
	stTime time = CTimeManager::GetInstance()->GetTime();
	_tprintf(TEXT("%lld:%lldÃÊ\n\n\n"), time.m_ullSecond, time.m_ullMillsecond);
	
	RESULT_CODE *resultArr = CGameLogic::GetInstance()->GetResultArr();
	for (int i = 0; i < sizeof(g_Timing) / sizeof(int); i++)
	{
		_tprintf(TEXT("%02d : %s\n"), g_Timing[i], g_ResStr[(int)resultArr[i]]);
	}

	system("cls");
}
