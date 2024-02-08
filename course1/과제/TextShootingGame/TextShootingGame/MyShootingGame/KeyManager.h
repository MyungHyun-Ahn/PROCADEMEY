#pragma once

#define KEY_CHECK(key, state) GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

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

struct stKeyInfo
{
	KEY_STATE		m_eState;
	bool			m_bPrevPush;	// 이전에 눌렸는지 여부
};

extern int g_arrVKKey[(int)KEY::END];

extern stKeyInfo g_KeyInfo[(int)KEY::END];

void KeyInit();

void KeyUpdate();

KEY_STATE GetKeyState(KEY key);