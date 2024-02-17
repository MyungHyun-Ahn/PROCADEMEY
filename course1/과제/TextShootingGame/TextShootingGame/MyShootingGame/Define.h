#pragma once

#define MS_PER_SEC 1000
#define MAX_FILE_NAME 256

typedef void (*fixedUpdate)(void);

enum class KEY_STATE
{
	TAP,
	HOLD,
	AWAY,
	NONE
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q, W, E, R,
	D, F,
	CTRL,
	SPACE,
	ESC,

	END
};

enum class SCENE_CODE
{
	LOBBY = 100,
	LOADING = 110,

	GAME = 500,

	CLEAR = 600,
	GAMEOVER = 700,

	END = 9999
};

struct stPos
{
	int m_iY;
	int m_iX;
};

// double type position
struct stPosD
{
	double m_dY;
	double m_dX;
};