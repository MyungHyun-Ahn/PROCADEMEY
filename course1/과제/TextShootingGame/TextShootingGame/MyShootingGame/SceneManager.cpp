#include "pch.h"
#include "SceneManager.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"
#include "FileLoader.h"

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

	[[fallthrough]];
	case SCENE_CODE::STAGE1:
	case SCENE_CODE::STAGE2:
	case SCENE_CODE::STAGE3:
		SceneLogic();
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
	wchar_t *loadbuffer = FileLoad(L"Resources\\Scene\\LOBBY_SCENE.txt");
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
		g_curScene.m_eNextScene = SCENE_CODE::STAGE1;
	}
}

void SceneLobbyRender(void)
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		wmemcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
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
	wchar_t *loadbuffer = FileLoad(L"Resources\\Scene\\LOADING_SCENE.txt");
	FileSceneParse(loadbuffer);
	SceneLoadingRender();
}

void SceneLoadingUpdate(void)
{
	// ���� �ε� �۾�

	// enemy �ε�
}

void SceneLoadingRender(void)
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		wmemcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
	ConsoleBufferFlip();
}

void SceneLoad(void)
{
	// ���� �ε� �۾�


	// �ε��� ������ �ʱ�ȭ
	EnemyInit(48);
	MissileInit();
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
	PlayerUpdate();
	EnemyUpdate();
	MissileUpdate();
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

