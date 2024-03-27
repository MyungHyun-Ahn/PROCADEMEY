#include "pch.h"
#include "Define.h"
#include "GameSetting.h"
#include "Scene.h"
#include "GameScene.h"
#include "PrintScene.h"
#include "BaseObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Missile.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "TimeManager.h"

TimeManager *g_TimeMgr;

TimeManager::TimeManager()
{
	Init();
}

TimeManager::~TimeManager()
{

}

void TimeManager::Init()
{
	timeBeginPeriod(dfTIME_PERIOD);
	m_iPrevTimeFrame = timeGetTime();
	m_iPrevTimeSec = m_iPrevTimeFrame;
	m_iCurTime = m_iPrevTimeSec;

	m_iUpdateFps = 0;
	m_iRenderFps = 0;

	m_uliStartTime = GetTickCount64();
	Update();
}

void TimeManager::Release()
{
	timeEndPeriod(dfTIME_PERIOD);
}

void TimeManager::Update()
{
	m_uliCurTime = (int)(GetTickCount64() - m_uliStartTime);
	m_iCurTime = timeGetTime();
	if (m_iCurTime = timeGetTime(); m_iCurTime - m_iPrevTimeSec >= 1000)
	{
		char title[256];
		char stageName[20];

		switch (SCENE_CODE code = g_SceneMgr->GetSceneCode(); code)
		{
		case SCENE_CODE::LOBBY:
			memcpy_s(stageName, 20, "LOBBY", 6);
			break;
		case SCENE_CODE::LOADING:
			memcpy_s(stageName, 20, "LOADING", 8);
			break;
		case SCENE_CODE::GAME:
		{
			char *sceneName = g_SceneMgr->GetCurrentSceneName();
			memcpy_s(stageName, 20, sceneName, strlen(sceneName) - 4); // .txt 빼기
			stageName[strlen(sceneName) - 4] = '\0';
		}
		break;
		case SCENE_CODE::CLEAR:
			memcpy_s(stageName, 20, "GAME_CLEAR", 11);
			break;
		case SCENE_CODE::GAMEOVER:
			memcpy_s(stageName, 20, "GAMEOVER", 9);
			break;
		}
		sprintf_s(title, "Text Shooting Game : Stage : %s, Update : %d, Render : %d", stageName, m_iUpdateFps, m_iRenderFps);
		SetConsoleTitleA(title);
		m_iPrevTimeSec += 1000;
		m_iUpdateFps = 0;
		m_iRenderFps = 0;
	}
}

bool TimeManager::CalCoolTime(unsigned int &prevTime, int coolTime)
{
	if (GetTimeDiff(prevTime) < coolTime)
	{
		return false;
	}

	prevTime = m_uliCurTime; // 현재 시간으로 갱신
	return true;
}
