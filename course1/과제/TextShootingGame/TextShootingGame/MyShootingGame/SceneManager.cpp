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

stScene g_curScene;

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
	// 기존에 있던 버퍼는 비워주기
	if (g_curScene.m_pBuffer != nullptr)
	{
		free(g_curScene.m_pBuffer);
		g_curScene.m_pBuffer = nullptr;
	}
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

	// enemy 배치
	EnemyInit();
	TimerInit(); // 쿨타임 계산을 위해 타이머를 다시 초기화

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

void SceneLoad(void)
{
	// 파일 로드 작업


	// 로드한 정보로 초기화
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
}

// Render
void SceneGameRender(void)
{
	ConsoleBufferClear();
	MissileRender();
	EnemyRender();
	ConsoleBufferFlip();
}

void SceneGame(void)
{
	SceneInput();
	SceneGameUpdate();
	SceneGameRender();
	Sleep(15);
}

