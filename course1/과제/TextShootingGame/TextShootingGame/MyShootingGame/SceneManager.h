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

// Scene GameOver

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