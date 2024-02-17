#pragma once
struct stScene
{
	char m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // 화면 버퍼
	SCENE_CODE m_eCurScene;
};

// Scene Init - 초기 상태 로드
void SceneInit(void);

// Scene Lobby
void SceneLobby(void);
void SceneLobbyUpdate(void);
void SceneLobbyRender(void);

// Scene Clear
void SceneClear(void);
void SceneClearInit(void);
void SceneClearUpdate(void);
void SceneClearRender(void);

// Scene GameOver
void SceneGameOver(void);
void SceneGameOverInit(void);
void SceneGameOverUpdate(void);
void SceneGameOverRender(void);

// Scene Loading
void SceneLoading(void);
void SceneLoadingUpdate(void);
void SceneLoadingRender(void);

// Scene Main
void SceneMain(void);

// Scene Logic
void SceneInput(void);
void SceneGameUpdate(void);
void SceneGameRender(void);

SCENE_CODE SceneCheckGameStatus(void);

void SceneGame(void);

// 게임 리셋
void SceneGameReset(void);

// 프로그램 종료 커맨드
void SceneExitCommand(void);

// 편의 기능 - 스테이지 스킵
void SceneStageSkip(void);