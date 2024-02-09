#include "pch.h"
#include "Core.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"

// 초기화
void Init(void)
{
	ConsoleInitial();
	KeyInit();
	TimerInit();
	PlayerInit(dfSCREEN_HEIGHT / 2, dfSCREEN_WIDTH / 2, 500);
	EnemyInit(48);
	MissileInit();
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
	// GetKeyState로 입력 체크
	PlayerUpdate();
	EnemyUpdate();
	MissileUpdate();
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
		Input();
		Update();
		Render();
		Sleep(40);
	}
}
