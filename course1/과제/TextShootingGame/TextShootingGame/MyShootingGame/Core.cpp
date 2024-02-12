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

// �ʱ�ȭ
void Init(void)
{
	ConsoleInitial();
	SceneInit();
	KeyInit();
	TimerInit();
	FileStageInfoParse("Resources\\Stage\\STAGE_INFO.txt");
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
