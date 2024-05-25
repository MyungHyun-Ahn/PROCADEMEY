#pragma once
struct stKeyInfo
{
	KEY_STATE		m_eState;
	bool			m_bPrevPush;
};

class KeyManager
{
	SINGLE(KeyManager)

public:
	void Init();
	void Update();

	KEY_STATE GetKeyState(KEY key);

private:
	stKeyInfo m_KeyInfo[(int)KEY::END];
};

extern int g_arrVKKey[(int)KEY::END];
extern KeyManager *g_KeyMgr;

#define KEY_CHECK(key, state) g_KeyMgr->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)