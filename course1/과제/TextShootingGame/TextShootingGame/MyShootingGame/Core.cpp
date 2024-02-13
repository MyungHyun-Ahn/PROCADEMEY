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

// √ ±‚»≠
void Init(void)
{
	ConsoleInitial();
	SceneInit();
	KeyInit();
	TimerInit();
	FileStageInfoParse("Resources\\Stage\\STAGE_INFO.txt");
}

void GameMain(void)
{
	while (true)
	{
		SceneMain();
		Sleep(40);
	}
}
