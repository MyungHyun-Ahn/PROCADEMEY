#include "pch.h"
#include "SceneManager.h"
#include "Console.h"
#include "KeyManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "TimeManager.h"
#include "FileLoader.h"
#include "StageInfo.h"

void SceneMain(void)
{
	// ��� ��ȯ switch case ��
	switch (g_curScene.m_eCurScene)
	{
	case SCENE_CODE::LOBBY:
		SceneLobby();
		break;
	case SCENE_CODE::LOADING:
		SceneLoading();
		break;
	case SCENE_CODE::GAME:
		SceneGame();
		break;
	case SCENE_CODE::CLEAR:
		SceneClear();
		break;
	case SCENE_CODE::GAMEOVER:
		SceneGameOver();
		break;
	case SCENE_CODE::END:
		break;
	}
	SceneExitCommand();
}

void SceneInit(void)
{
	// �κ� Scene �ε�
	g_curScene.m_eCurScene = SCENE_CODE::LOBBY;
	FileSceneParse("Resources\\Scene\\LOBBY_SCENE.txt");
}

void SceneLobby(void)
{
	SceneInput();
	TimerFixedUpdate(SceneLobbyUpdate);
	SceneLobbyRender();
	TimerRenderCheck();
}

void SceneLobbyUpdate(void)
{
	if (KEY_TAP(KEY::SPACE))
	{
		// Scene ��ȯ
		g_curScene.m_eCurScene = SCENE_CODE::LOADING; // Loading

		// Scene ��ȯ�Ǹ鼭 Player �ʱ�ȭ
		PlayerInit();
	}
}

void SceneLobbyRender(void)
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
	ConsoleBufferFlip();
}

void SceneClear(void)
{
	SceneInput();
	TimerFixedUpdate(SceneClearUpdate);
	SceneClearRender();
	TimerRenderCheck();
}

void SceneClearInit(void)
{
	SceneGameReset();
	FileSceneParse("Resources\\Scene\\CLEAR_SCENE.txt");
}

void SceneClearUpdate(void)
{
	if (KEY_TAP(KEY::SPACE))
	{
		// Scene ��ȯ
		g_curScene.m_eCurScene = SCENE_CODE::LOBBY; // Loading

		SceneInit();
	}
}

void SceneClearRender(void)
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
	ConsoleBufferFlip();
}

void SceneGameOver(void)
{
	SceneInput();
	TimerFixedUpdate(SceneGameOverUpdate);
	SceneGameOverRender();
	TimerRenderCheck();
}

void SceneGameOverInit(void)
{
	SceneGameReset();
	FileSceneParse("Resources\\Scene\\GAMEOVER_SCENE.txt");
}

void SceneGameOverUpdate(void)
{
	if (KEY_TAP(KEY::SPACE))
	{
		// Scene ��ȯ
		g_curScene.m_eCurScene = SCENE_CODE::LOBBY; // Loading

		SceneInit();
	}
}

void SceneGameOverRender(void)
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
	ConsoleBufferFlip();
}

void SceneLoading(void)
{
	FileSceneParse("Resources\\Scene\\LOADING_SCENE.txt");
	SceneLoadingRender();
	// �� �ε�
	SceneLoadingUpdate();
}

// ���� �ε� �۾�
void SceneLoadingUpdate(void)
{
	// Stage Scene �ε�
	FileStageParse(g_StageInfos[g_iCurStage].m_chFileName);
	
	// ���� ������������ �Ҵ��� �� �Ҵ� ����
	EnemyRelease();
	MissileRelease();

	// enemy �ε�
	// �ѹ� �ε�� enemy�� ������ ����� ������ �ٽ� �ε����� �ʴ´�.
	// map�� �����س��� �̹� �ε�� �����͸� �ҷ��ͼ� ���
	for (int i = 0; i < g_StageInfos[g_iCurStage].m_iEnemyTypeCount; i++)
	{
		FileEnemyParse(g_StageInfos[g_iCurStage].m_arrEnemys[i]);
	}

	// enemy ��ġ
	EnemyInit();
	TimerInit(); // ��Ÿ�� ����� ���� Ÿ�̸Ӹ� �ٽ� �ʱ�ȭ
	PlayerCooltimeReset(); // ��Ÿ���� 0���� ����ϹǷ� �ʱ�ȭ

	g_curScene.m_eCurScene = SCENE_CODE::GAME; // ���� ������ ��ȯ
}

void SceneLoadingRender(void)
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(g_szScreenBuffer[iCnt], dfSCREEN_WIDTH, g_curScene.m_szConsoleBuffer[iCnt], dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
	ConsoleBufferFlip();
}

// Input
void SceneInput(void)
{
	// Ű���� ���� ������Ʈ
	KeyUpdate();
	TimerUpdate();
}

// Update - ���� ����
void SceneGameUpdate(void)
{
	EnemyUpdate();
	MissileUpdate();
	PlayerUpdate();

#ifdef _DEBUG
	SceneStageSkip();
#endif
}

// Render
void SceneGameRender(void)
{
	ConsoleBufferClear();
	MissileRender();
	EnemyRender();
	PlayerRender();
	ConsoleBufferFlip();
}

SCENE_CODE SceneCheckGameStatus(void)
{
	if (g_bIsGameOver)
	{
		SceneGameOverInit();
		return SCENE_CODE::GAMEOVER;
	}

	if (g_bIsStageClear)
	{
		g_iCurStage++;
		if (g_iCurStage == g_iStageCount)
		{
			SceneClearInit();
			return SCENE_CODE::CLEAR;
		}

		return SCENE_CODE::LOADING;
	}

	// ���� GAME ���� ����
	return SCENE_CODE::GAME;
}

void SceneGame(void)
{
	SceneInput();

	TimerFixedUpdate(SceneGameUpdate);

	SceneGameRender();

	if (SCENE_CODE code = SceneCheckGameStatus(); code != SCENE_CODE::GAME)
	{
		g_curScene.m_eCurScene = code;
	}

	TimerRenderCheck();
}

void SceneGameReset(void)
{
	g_iCurStage = 0;
	EnemyRelease();
	MissileRelease();
}

void SceneExitCommand(void)
{
	if (!(KEY_TAP(KEY::ESC)))
		return;


	// ����� �� ����Ǿ�� �ϴ� ��
	EnemyRelease();
	MissileRelease();

	g_bProgramExit = true;

	if (g_StageInfos != nullptr)
	{
		for (int i = 0; i < g_iStageCount; i++)
		{
			if (g_StageInfos[i].m_chFileName != nullptr)
			{
				free(g_StageInfos[i].m_chFileName);
				g_StageInfos[i].m_chFileName = nullptr;
			}

			if (g_StageInfos[i].m_arrEnemys != nullptr)
			{
				free(g_StageInfos[i].m_arrEnemys);
				g_StageInfos[i].m_arrEnemys = nullptr;
			}
		}

		free(g_StageInfos);
		g_StageInfos = nullptr;
	}

	for (auto it = g_mapEnemyInfos.begin(); it != g_mapEnemyInfos.end(); ++it)
	{
		stEnemyInfo *enemyInfo = it->second;

		if (enemyInfo != nullptr)
		{
			if (enemyInfo->m_MissileInfos != nullptr)
			{
				for (int i = 0; i < enemyInfo->m_iMissileCount; i++)
				{
					if (enemyInfo->m_MissileInfos[i].m_MissileMoves != nullptr)
					{
						free(enemyInfo->m_MissileInfos[i].m_MissileMoves);
						enemyInfo->m_MissileInfos[i].m_MissileMoves = nullptr;
					}
				}
				
				free(enemyInfo->m_MissileInfos);
				enemyInfo->m_MissileInfos = nullptr;
			}

			if (enemyInfo->m_Moves != nullptr)
			{
				free(enemyInfo->m_Moves);
				enemyInfo->m_Moves = nullptr;
			}

			free(enemyInfo);
			enemyInfo = nullptr;
		}
	}

	g_mapEnemyInfos.clear();
}

void SceneStageSkip(void)
{
	if (!(KEY_TAP(KEY::CTRL)))
		return;

	g_bIsStageClear = true;
}

