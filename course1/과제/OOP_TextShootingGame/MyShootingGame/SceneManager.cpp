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
#include "UiManager.h"

SceneManager *g_SceneMgr;

SceneManager::SceneManager() 
{
	RegisterStageInfo();
	LoadScene(SCENE_CODE::LOBBY);
}

SceneManager::~SceneManager()
{

}

void SceneManager::Update()
{
	m_curScene->Update();
}

void SceneManager::Render()
{
	m_curScene->Render();
}

void SceneManager::RegisterStageInfo()
{
	m_StageInfos = g_FileMgr->StageInfoParse("Resources\\Stage\\STAGE_INFO.txt", m_StageCount);
	if (m_StageInfos == nullptr)
		throw;
}

void SceneManager::LoadScene(SCENE_CODE code)
{
	m_SceneCode = code;
	switch (m_SceneCode)
	{
	case SCENE_CODE::LOBBY:
	{
		Scene *delScene = m_curScene;
		m_curScene = new PrintScene();
		m_curScene->Register(g_FileMgr->SceneLoad("Resources\\Scene\\LOBBY_SCENE.txt"));
		delete delScene;
	}
		break;
	case SCENE_CODE::LOADING:
	{
		Scene *delScene = m_curScene;
		m_curScene = new PrintScene();
		m_curScene->Register(g_FileMgr->SceneLoad("Resources\\Scene\\LOADING_SCENE.txt"));
		delete delScene;
	}
		break;
	case SCENE_CODE::GAME:
	{
		if (m_StageInfos == nullptr)
			RegisterStageInfo();

		if (m_CurStageNum == 0)
		{
			g_ObjMgr->CreatePlayer();
		}

		// 스테이지 번호 증가
		m_CurStageNum++;

		if (m_CurStageNum > m_StageCount)
		{
			// 클리어로 이동
			LoadScene(SCENE_CODE::CLEAR);
			break;
		}

		Scene *delScene = m_curScene;
		m_curScene = new GameScene();
		m_curScene->Register(g_FileMgr->StageLoad(m_StageInfos[m_CurStageNum - 1].m_szFileName));
		m_iEnemyCount = m_StageInfos[m_CurStageNum - 1].m_iEnemyCount;
		m_iCurEnemyCount = m_StageInfos[m_CurStageNum - 1].m_iEnemyCount;
		g_UiMgr->SendMaxEnemyCountData(m_iEnemyCount);
		g_UiMgr->SendCurEnemyCountData(m_iCurEnemyCount);

		delete delScene;
	}
		break;
	case SCENE_CODE::CLEAR:
	{
		Scene *delScene = m_curScene;
		m_curScene = new PrintScene();
		g_ObjMgr->ResetObject(false);
		m_curScene->Register(g_FileMgr->SceneLoad("Resources\\Scene\\CLEAR_SCENE.txt"));
		delete delScene;
		m_CurStageNum = 0;
	}
		break;
	case SCENE_CODE::GAMEOVER:
	{
		Scene *delScene = m_curScene;
		m_curScene = new PrintScene();
		m_curScene->Register(g_FileMgr->SceneLoad("Resources\\Scene\\GAMEOVER_SCENE.txt"));
		delete delScene;
		m_CurStageNum = 0;
	}
		break;
	case SCENE_CODE::END:
		break;
	}
}

void SceneManager::ResetStageInfo()
{
	if (m_StageInfos != nullptr)
	{
		for (int i = 0; i < m_StageCount; i++)
		{
			if (m_StageInfos[i].m_szFileName != nullptr)
			{
				delete m_StageInfos[i].m_szFileName;
			}

			if (m_StageInfos[i].m_arrEnemys != nullptr)
			{
				delete m_StageInfos[i].m_arrEnemys;
			}
		}

		delete m_StageInfos;
	}
}