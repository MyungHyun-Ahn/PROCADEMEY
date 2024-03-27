#include "pch.h"
#include "Define.h"
#include "GameSetting.h"
#include "Enemy.h"
#include "Player.h"
#include "Missile.h"
#include "Scene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "ConsoleManager.h"
#include "FileManager.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "ObjectManager.h"
#include "Core.h"

Core *g_Core = Core::GetInstance();

Core::Core() {}
Core::~Core() {}

void FixedUpdate()
{
	g_Core->Update();
}

void Core::Init()
{
	g_KeyMgr = KeyManager::GetInstance();
	g_TimeMgr = TimeManager::GetInstance();
	g_ProfileMgr = CProfileManager::GetInstance();
	g_FileMgr = FileManager::GetInstance();
	g_SceneMgr = SceneManager::GetInstance();
	g_ObjMgr = ObjectManager::GetInstance();
}

void Core::Start()
{
	Init();

	while (true)
	{
		Input();
		g_TimeMgr->FixedUpdate(FixedUpdate);
		Render();
		g_TimeMgr->RenderCheck();
	}
}

void Core::Input()
{
	g_TimeMgr->Update();
	g_KeyMgr->Update();
}

void Core::Update()
{
	g_SceneMgr->Update();
}

void Core::Render()
{
	g_SceneMgr->Render();
}
