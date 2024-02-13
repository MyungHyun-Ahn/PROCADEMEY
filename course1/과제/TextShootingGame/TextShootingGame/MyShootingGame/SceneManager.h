#pragma once

enum class SCENE_CODE
{
	LOBBY = 100,
	LOADING = 110,

	GAME = 500,

	CLEAR = 600,
	GAMEOVER = 700,

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
	char *m_pBuffer; // 파일 로드 버퍼
	char m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // 화면 버퍼
	SCENE_CODE m_eCurScene;
};

// 전역 변수로 제어
extern stScene g_curScene;


// Scene Init - 초기 상태 로드
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
void SceneGameUpdate(void);
void SceneGameRender(void);

void SceneGame(void);