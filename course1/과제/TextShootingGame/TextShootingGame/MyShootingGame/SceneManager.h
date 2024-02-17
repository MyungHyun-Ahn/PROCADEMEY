#pragma once
struct stScene
{
	char m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // ȭ�� ����
	SCENE_CODE m_eCurScene;
};

// Scene Init - �ʱ� ���� �ε�
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

// ���� ����
void SceneGameReset(void);

// ���α׷� ���� Ŀ�ǵ�
void SceneExitCommand(void);

// ���� ��� - �������� ��ŵ
void SceneStageSkip(void);