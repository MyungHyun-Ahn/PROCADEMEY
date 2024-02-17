#include "pch.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "StageInfo.h"

void TimerInit(void)
{
    timeBeginPeriod(dfTIME_PERIOD);
    g_stTimer.m_iPrevTimeFrame = timeGetTime();
    g_stTimer.m_iPrevTimeSec = g_stTimer.m_iPrevTimeFrame;
    g_stTimer.m_iCurTime = g_stTimer.m_iCurTime;
    
	g_stTimer.m_iUpdateFps = 0;
	g_stTimer.m_iRenderFps = 0;

    g_stTimer.m_uliStartTime = GetTickCount64();
    TimerUpdate();
}

void TimerRelease(void)
{
    timeEndPeriod(dfTIME_PERIOD);
}

void TimerUpdate(void)
{
	// 시작 시간을 빼서 0부터 얼마나 시간이 흘렀는지를 측정
	g_stTimer.m_uliCurTime = (int)(GetTickCount64() - g_stTimer.m_uliStartTime);

	g_stTimer.m_iCurTime = timeGetTime();
	if (g_stTimer.m_iCurTime - g_stTimer.m_iPrevTimeSec >= 1000)
	{
		char title[256];
		char stageName[20];

		switch (g_curScene.m_eCurScene)
		{
		case SCENE_CODE::LOBBY:
			memcpy_s(stageName, 20, "LOBBY", 6);
			break;
		case SCENE_CODE::LOADING:
			memcpy_s(stageName, 20, "LOADING", 8);
			break;
		case SCENE_CODE::GAME:
			memcpy_s(stageName, 20, g_StageInfos[g_iCurStage].m_szFileName, strlen(g_StageInfos[g_iCurStage].m_szFileName) - 4); // .txt 빼기
			stageName[strlen(g_StageInfos[g_iCurStage].m_szFileName) - 4] = '\0';
			break;
		case SCENE_CODE::CLEAR:
			memcpy_s(stageName, 20, "GAME_CLEAR", 11);
			break;
		case SCENE_CODE::GAMEOVER:
			memcpy_s(stageName, 20, "GAMEOVER", 9);
			break;
		}
		sprintf_s(title, "Text Shooting Game : Stage : %s, Update : %d, Render : %d", stageName, g_stTimer.m_iUpdateFps, g_stTimer.m_iRenderFps);
		SetConsoleTitleA(title);
		g_stTimer.m_iPrevTimeSec += 1000;
		g_stTimer.m_iUpdateFps = 0;
		g_stTimer.m_iRenderFps = 0;
	}
}

// 인자로 받은 쿨타임으로 지금 사용 가능한지 여부
// int coolTime : ms 단위
bool TimerCalCoolTime(unsigned int &prevTime, int coolTime)
{
    if (TimerGetTimeDiff(prevTime) < coolTime)
    {
        return false;
    }
   
    prevTime = g_stTimer.m_uliCurTime; // 현재 시간으로 갱신
    return true;
}
