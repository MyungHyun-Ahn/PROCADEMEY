#include "pch.h"
#include "Core.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "FileLoader.h"

// 초기화
void Init(void)
{
	ConsoleInitial();
	SceneInit();
	KeyInit();
	TimerInit();
	FileStageInfoParse("Resources\\Stage\\STAGE_INFO.txt");
}

// 입력
void Input(void)
{
	// 키보드 상태 업데이트
	KeyUpdate();
	TimerUpdate();
}

// 게임 로직 업데이트
void Update(void)
{
}

// 렌더
void Render(void)
{
	ConsoleRender();
}

void GameMain(void)
{
	while (true)
	{
		SceneMain();
		Sleep(40);
	}
}
