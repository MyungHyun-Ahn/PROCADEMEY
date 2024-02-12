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
	wchar_t *m_pBuffer; // 파일 로드 버퍼
	wchar_t m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // 화면 버퍼
	SCENE_CODE m_eCurScene;
	SCENE_CODE m_eNextScene;
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
void SceneUpdate(void);
void SceneRender(void);

void SceneLogic(void);