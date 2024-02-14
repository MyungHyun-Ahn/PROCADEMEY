#include "pch.h"
#include "Core.h"
#include "Console.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "FileLoader.h"

// 초기화 - 게임 시작 전 모든 사전 작업 진행
void Init(void)
{
	ConsoleInitial();
	SceneInit();
	KeyInit();
	TimerInit();
	FileStageInfoParse("Resources\\Stage\\STAGE_INFO.txt");
}

// GameMain - 무한 루프는 여기에서만 진행
void GameMain(void)
{
	Init();

	while (true)
	{
		SceneMain();

		if (g_bProgramExit)
			break;
	}
}
