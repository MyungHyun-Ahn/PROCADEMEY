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
	// 장면 전환 switch case 문
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
	// 로비 Scene 로드
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
		// Scene 전환
		g_curScene.m_eCurScene = SCENE_CODE::LOADING; // Loading

		// Scene 전환되면서 Player 초기화
		PlayerInit();
	}
}

void SceneLobbyRender(void)
{
	// 콘솔 버퍼에 쓰기
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
		// Scene 전환
		g_curScene.m_eCurScene = SCENE_CODE::LOBBY; // Loading

		SceneInit();
	}
}

void SceneClearRender(void)
{
	// 콘솔 버퍼에 쓰기
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
		// Scene 전환
		g_curScene.m_eCurScene = SCENE_CODE::LOBBY; // Loading

		SceneInit();
	}
}

void SceneGameOverRender(void)
{
	// 콘솔 버퍼에 쓰기
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
	// 씬 로드
	SceneLoadingUpdate();
}

// 게임 로딩 작업
void SceneLoadingUpdate(void)
{
	// Stage Scene 로딩
	FileStageParse(g_StageInfos[g_iCurStage].m_chFileName);
	
	// 이전 스테이지에서 할당한 것 할당 해제
	EnemyRelease();
	MissileRelease();

	// enemy 로딩
	// 한번 로드된 enemy는 게임이 종료될 때까지 다시 로드하지 않는다.
	// map에 저장해놓고 이미 로드된 데이터를 불러와서 사용
	for (int i = 0; i < g_StageInfos[g_iCurStage].m_iEnemyTypeCount; i++)
	{
		FileEnemyParse(g_StageInfos[g_iCurStage].m_arrEnemys[i]);
	}

	// enemy 배치
	EnemyInit();
	TimerInit(); // 쿨타임 계산을 위해 타이머를 다시 초기화
	PlayerCooltimeReset(); // 쿨타임을 0부터 계산하므로 초기화

	g_curScene.m_eCurScene = SCENE_CODE::GAME; // 게임 씬으로 전환
}

void SceneLoadingRender(void)
{
	// 콘솔 버퍼에 쓰기
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
	// 키보드 상태 업데이트
	KeyUpdate();
	TimerUpdate();
}

// Update - 게임 로직
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

	// 현재 GAME 상태 유지
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


	// 종료될 때 수행되어야 하는 것
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

