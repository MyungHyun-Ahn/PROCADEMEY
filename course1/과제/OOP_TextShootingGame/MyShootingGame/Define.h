#pragma once

constexpr int MS_PER_SEC = 1000;
constexpr int MAX_FILE_NAME = 256;

// ΩÃ±€≈Ê Define
#define SINGLE(type)		private:										\
								type();										\
								~type();									\
								inline static type *m_instPtr = nullptr;	\
							public:											\
								static type *GetInstance()					\
								{											\
									if (m_instPtr == nullptr)				\
									{										\
										m_instPtr = new type();				\
										atexit(Destory);					\
									}										\
																			\
									return m_instPtr;						\
								}											\
																			\
								static void Destory()						\
								{											\
									delete m_instPtr;						\
									m_instPtr = nullptr;					\
								}


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

#define POS_DTOI(pos) stPos{(int)pos.m_dY, (int)pos.m_dX}