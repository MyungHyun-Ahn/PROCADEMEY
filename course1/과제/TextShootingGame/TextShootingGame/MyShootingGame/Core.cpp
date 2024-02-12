#include "pch.h"
#include "Core.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"
#include "SceneManager.h"

// �ʱ�ȭ
void Init(void)
{
	ConsoleInitial();
	SceneInit();
	KeyInit();
	TimerInit();
	PlayerInit(dfSCREEN_HEIGHT / 2, dfSCREEN_WIDTH / 2, 500);
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
		SceneMain();
		Sleep(40);
	}
}
