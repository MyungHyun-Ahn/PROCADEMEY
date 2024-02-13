#include "pch.h"
#include "SceneManager.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"
#include "FileLoader.h"
#include "StageInfo.h"

void SceneMain(void)
{
	// 장면 전환 switch case 문
	switch (g_curScene.m_eCurScene)
	{
	case SCENE_CODE::LOBBY:
		SceneLobby();
		break;
	case SCENE_CODE::LOADING:
		SceneLoading();
		break;
	case SCENE_CODE::GAME:
		SceneGame();
		break;
	case SCENE_CODE::CLEAR:
		break;
	case SCENE_CODE::GAMEOVER:
		break;
	case SCENE_CODE::END:
		break;
	}

}

void SceneInit(void)
{
	// 로비 Scene 로드
	g_curScene.m_eCurScene = SCENE_CODE::LOBBY;
	FileSceneParse("Resources\\Scene\\LOBBY_SCENE.txt");
}

void SceneLobby(void)
{
	SceneInput();
	SceneLobbyUpdate();
	SceneLobbyRender();
}

void SceneLobbyUpdate(void)
{
	if (KEY_TAP(KEY::SPACE))
	{
		// Scene 전환
		g_curScene.m_eCurScene = SCENE_CODE::LOADING; // Loading
	}
}

void SceneLobbyRender(void)
{
	// 콘솔 버퍼에 쓰기
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
	ConsoleBufferFlip();
}

void SceneLoading(void)
{
	FileSceneParse("Resources\\Scene\\LOADING_SCENE.txt");
	SceneLoadingRender();

	// 씬 로드
	SceneLoadingUpdate();
}

// 게임 로딩 작업
void SceneLoadingUpdate(void)
{
	// Stage Scene 로딩
	FileStageParse(g_StageInfos[g_iCurStage].m_chFileName);
	
	// enemy 로딩
	for (int i = 0; i < g_StageInfos[g_iCurStage].m_iEnemyCount; i++)
	{
		FileEnemyParse(g_StageInfos[g_iCurStage].m_arrEnemys[i]);
	}

	// 이전 스테이지에서 할당한 것 할당 해제
	EnemyRelease();

	// enemy 배치
	EnemyInit();
	TimerInit(); // 쿨타임 계산을 위해 타이머를 다시 초기화
	PlayerCooltimeReset(); // 쿨타임을 0부터 계산하므로 초기화

	g_curScene.m_eCurScene = SCENE_CODE::GAME; // 게임 씬으로 전환
}

void SceneLoadingRender(void)
{
	// 콘솔 버퍼에 쓰기
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
	ConsoleBufferFlip();
}

// Input
void SceneInput(void)
{
	// 키보드 상태 업데이트
	KeyUpdate();
	TimerUpdate();
}

// Update - 게임 로직
void SceneGameUpdate(void)
{
	EnemyUpdate();
	MissileUpdate();
	PlayerUpdate();
}

// Render
void SceneGameRender(void)
{
	ConsoleBufferClear();
	MissileRender();
	EnemyRender();
	PlayerRender();
	ConsoleBufferFlip();
}

SCENE_CODE SceneCheckGameStatus(void)
{
	if (g_bIsGameOver)
	{
		return SCENE_CODE::GAMEOVER;
	}

	if (g_bIsStageClear)
	{
		g_iCurStage++;
		if (g_iCurStage == g_iStageCount)
		{
			return SCENE_CODE::CLEAR;
		}

		return SCENE_CODE::LOADING;
	}

	// 현재 GAME 상태 유지
	return SCENE_CODE::GAME;
}

void SceneGame(void)
{
	SceneInput();
	SceneGameUpdate();
	SceneGameRender();

	if (SCENE_CODE code = SceneCheckGameStatus(); code != SCENE_CODE::GAME)
	{
		g_curScene.m_eCurScene = code;
	}
	Sleep(40);
}

