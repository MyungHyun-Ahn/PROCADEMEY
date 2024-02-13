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
	// ��� ��ȯ switch case ��
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
	// �κ� Scene �ε�
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
		// Scene ��ȯ
		g_curScene.m_eCurScene = SCENE_CODE::LOADING; // Loading
	}
}

void SceneLobbyRender(void)
{
	// �ܼ� ���ۿ� ����
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

	// �� �ε�
	SceneLoadingUpdate();
}

// ���� �ε� �۾�
void SceneLoadingUpdate(void)
{
	// Stage Scene �ε�
	FileStageParse(g_StageInfos[g_iCurStage].m_chFileName);
	
	// enemy �ε�
	for (int i = 0; i < g_StageInfos[g_iCurStage].m_iEnemyCount; i++)
	{
		FileEnemyParse(g_StageInfos[g_iCurStage].m_arrEnemys[i]);
	}

	// ���� ������������ �Ҵ��� �� �Ҵ� ����
	EnemyRelease();

	// enemy ��ġ
	EnemyInit();
	TimerInit(); // ��Ÿ�� ����� ���� Ÿ�̸Ӹ� �ٽ� �ʱ�ȭ
	PlayerCooltimeReset(); // ��Ÿ���� 0���� ����ϹǷ� �ʱ�ȭ

	g_curScene.m_eCurScene = SCENE_CODE::GAME; // ���� ������ ��ȯ
}

void SceneLoadingRender(void)
{
	// �ܼ� ���ۿ� ����
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
	// Ű���� ���� ������Ʈ
	KeyUpdate();
	TimerUpdate();
}

// Update - ���� ����
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

	// ���� GAME ���� ����
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

