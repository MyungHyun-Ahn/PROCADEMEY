#include "pch.h"
#include "Core.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "FileLoader.h"

// �ʱ�ȭ - ���� ���� �� ��� ���� �۾� ����
void Init(void)
{
	ConsoleInitial();
	SceneInit();
	KeyInit();
	PlayerInit();
	FileStageInfoParse("Resources\\Stage\\STAGE_INFO.txt");
}

// GameMain - ���� ������ ���⿡���� ����
void GameMain(void)
{
	Init();

	while (true)
	{
		SceneMain();
	}
}
