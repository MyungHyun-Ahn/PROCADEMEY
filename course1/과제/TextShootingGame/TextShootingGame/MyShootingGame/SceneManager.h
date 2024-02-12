#pragma once

enum class SCENE_CODE
{
	LOBBY = 100,
	LOADING = 110,

	STAGE1 = 501,
	STAGE2 = 502,
	STAGE3 = 503,

	END = 9999
};

enum class SCENE_STATUS
{
	LOADING,
	LOAD_COMPLETE,
	CLEAR
};

struct stScene
{
	wchar_t *m_pBuffer; // ���� �ε� ����
	wchar_t m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // ȭ�� ����
	SCENE_CODE m_eCurScene;
	SCENE_CODE m_eNextScene;
};

// ���� ������ ����
extern stScene g_curScene;


// Scene Init - �ʱ� ���� �ε�
void SceneInit(void);

// Scene Lobby
void SceneLobby(void);
void SceneLobbyUpdate(void);
void SceneLobbyRender(void);

// Scene Loading
void SceneLoading(void);
void SceneLoadingUpdate(void);
void SceneLoadingRender(void);

// Scene Main
void SceneMain(void);

// Scene Logic
void SceneLoad(void);

void SceneInput(void);
void SceneUpdate(void);
void SceneRender(void);

void SceneLogic(void);