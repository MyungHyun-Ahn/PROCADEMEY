#include "pch.h"
#include "Core.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"

// �ʱ�ȭ
void Init(void)
{
	ConsoleInitial();
	KeyInit();
	TimerInit();
	PlayerInit(dfSCREEN_HEIGHT / 2, dfSCREEN_WIDTH / 2, 500);
	EnemyInit(48);
	MissileInit();
}

// �Է�
void Input(void)
{
	// Ű���� ���� ������Ʈ
	KeyUpdate();
	TimerUpdate();
}

// ���� ���� ������Ʈ
void Update(void)
{
	// GetKeyState�� �Է� üũ
	PlayerUpdate();
	EnemyUpdate();
	MissileUpdate();
}

// ����
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
