#include "pch.h"
#include "Define.h"
#include "KeyManager.h"

KeyManager *g_KeyMgr;

int g_arrVKKey[(int)KEY::END] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	// 'Q', 'W', 'E', 'R',
	// 'D', 'F',
	// 
	// VK_CONTROL,
	// VK_SPACE,
	// VK_ESCAPE
};

KeyManager::KeyManager()
{
	Init();
}

KeyManager::~KeyManager()
{

}

void KeyManager::Init()
{
	// 키 배열 초기화
	for (int i = 0; i < (int)KEY::END; i++)
	{
		m_KeyInfo[i] = stKeyInfo{ KEY_STATE::NONE, false };
	}
}

// 프레임 당 키 상태 체크
void KeyManager::Update()
{
	for (int i = 0; i < (int)KEY::END; i++)
	{
		// 눌린 상태
		if (GetAsyncKeyState(g_arrVKKey[i]))
		{
			if (m_KeyInfo[i].m_bPrevPush)
			{
				// 이전에도 눌려있었다.
				m_KeyInfo[i].m_eState = KEY_STATE::HOLD;
			}
			else
			{
				// 이전에 눌려있지 않았다.
				m_KeyInfo[i].m_eState = KEY_STATE::TAP;
			}
			m_KeyInfo[i].m_bPrevPush = true;
		}
		else // 키가 눌려있지 않다.
		{
			if (m_KeyInfo[i].m_bPrevPush)
			{
				// 이전에는 눌려있었다.
				m_KeyInfo[i].m_eState = KEY_STATE::AWAY;
			}
			else
			{
				// 이전에도 안눌려있었다.
				m_KeyInfo[i].m_eState = KEY_STATE::NONE;
			}
			m_KeyInfo[i].m_bPrevPush = false;
		}
	}
}

KEY_STATE KeyManager::GetKeyState(KEY key)
{
	return m_KeyInfo[(int)key].m_eState;
}