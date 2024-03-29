#include "pch.h"
#include "Define.h"
#include "GameSetting.h"
#include "Scene.h"
#include "PrintScene.h"
#include "KeyManager.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "ConsoleManager.h"

void PrintScene::Update()
{
	SCENE_CODE code = g_SceneMgr->GetSceneCode();

	if (code == SCENE_CODE::LOADING)
	{
		g_SceneMgr->LoadScene(SCENE_CODE::GAME);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		switch (code)
		{
		case SCENE_CODE::LOBBY:
			g_SceneMgr->LoadScene(SCENE_CODE::LOADING);
			break;
		case SCENE_CODE::CLEAR:
			g_SceneMgr->LoadScene(SCENE_CODE::LOBBY);
			break;
		case SCENE_CODE::GAMEOVER:
			g_SceneMgr->LoadScene(SCENE_CODE::LOBBY);
			break;
		}
	}
}

void PrintScene::Render()
{
	g_ConsoleMgr->ConsoleWrite(m_szConsoleBuffer);

	g_ConsoleMgr->BufferFlip();
}

void PrintScene::Register(char *buffer)
{
	const char *del = "\r\n";
	char *nextTok = nullptr;
	char *token = strtok_s(buffer, del, &nextTok);

	int colNum = 0;
	// 버퍼 복사 TODO:
	while (token != nullptr)
	{
		memcpy_s(m_szConsoleBuffer[colNum++], dfSCREEN_WIDTH, token, dfSCREEN_WIDTH);
		token = strtok_s(nullptr, del, &nextTok);
	}

	delete buffer;
}
