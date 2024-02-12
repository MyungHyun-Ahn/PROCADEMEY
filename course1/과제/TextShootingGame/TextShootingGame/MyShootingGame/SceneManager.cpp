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
	// ��� ��ȯ switch case ��
	switch (g_curScene.m_eCurScene)
	{
	case SCENE_CODE::LOBBY:
		SceneLobby();
		break;
	case SCENE_CODE::LOADING:
		SceneLoading();
		break;
		break;
	case SCENE_CODE::END:
		break;
	}

}

void SceneInit(void)
{
	// �κ� Scene �ε�
	g_curScene.m_eCurScene = SCENE_CODE::LOBBY;
	g_curScene.m_eNextScene = SCENE_CODE::LOADING;
	char *loadbuffer = FileLoad("Resources\\Scene\\LOBBY_SCENE.txt");
	FileSceneParse(loadbuffer);
	free(loadbuffer);
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
		g_curScene.m_eCurScene = g_curScene.m_eNextScene; // Loading
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
	// ������ �ִ� ���۴� ����ֱ�
	if (g_curScene.m_pBuffer != nullptr)
	{
		free(g_curScene.m_pBuffer);
		g_curScene.m_pBuffer = nullptr;
	}
	char *loadbuffer = FileLoad("Resources\\Scene\\LOADING_SCENE.txt");
	FileSceneParse(loadbuffer);
	SceneLoadingRender();
	SceneLoadingUpdate();
}

void SceneLoadingUpdate(void)
{
	// ���� �ε� �۾�
	// enemy �ε�
	for (int i = 0; i < g_StageInfos[g_iCurStage].m_iEnemyCount; i++)
	{
		FileEnemyParse('R');
		// FileEnemyParse(g_StageInfos[g_iCurStage].m_arrEnemys[i]);
	}
	
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

void SceneLoad(void)
{
	// ���� �ε� �۾�


	// �ε��� ������ �ʱ�ȭ
}

// Scene ���� �帧
void SceneInput(void)
{
	// Ű���� ���� ������Ʈ
	KeyUpdate();
	TimerUpdate();
}

void SceneUpdate(void)
{
	// GetKeyState�� �Է� üũ
}

void SceneRender(void)
{
	ConsoleRender();
}

void SceneLogic(void)
{
	SceneInput();
	SceneUpdate();
	SceneRender();
	Sleep(40);
}

