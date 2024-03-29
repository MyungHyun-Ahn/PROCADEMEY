#include "pch.h"
#include "Define.h"
#include "GameSetting.h"
#include "Scene.h"
#include "BaseObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "ObjectManager.h"
#include "GameScene.h"
#include "FileManager.h"
#include "TimeManager.h"
#include "ConsoleManager.h"
#include "UiManager.h"

void GameScene::Update()
{
	g_ObjMgr->Update();
}

void GameScene::Render()
{
	g_ConsoleMgr->BufferClear();
	g_ObjMgr->Render();
	g_UiMgr->Render();
	g_ConsoleMgr->BufferFlip();
}

void GameScene::Register(char *buffer)
{
	// 콘솔 버퍼에 복사
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

	// 게임 초기화 작업
	g_TimeMgr->Init();
	g_ObjMgr->ResetObject(true);

	for (int y = 0; y < dfSCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < dfSCREEN_WIDTH; x++)
		{
			if (m_szConsoleBuffer[y][x] == ' ' || m_szConsoleBuffer[y][x] == '\0' || m_szConsoleBuffer[y][x] == '\n')
				continue;

			stEnemyInfo *info = g_FileMgr->EnemyParse(m_szConsoleBuffer[y][x]);
			g_ObjMgr->CreateEnemy({ y, x }, info);
		}
	}
}
