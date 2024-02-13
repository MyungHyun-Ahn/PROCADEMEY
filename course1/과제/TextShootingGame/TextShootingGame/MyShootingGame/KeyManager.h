#pragma once

#define KEY_CHECK(key, state) GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

struct stKeyInfo
{
	KEY_STATE		m_eState;
	bool			m_bPrevPush;	// 이전에 눌렸는지 여부
};

void KeyInit();

void KeyUpdate();

KEY_STATE GetKeyState(KEY key);