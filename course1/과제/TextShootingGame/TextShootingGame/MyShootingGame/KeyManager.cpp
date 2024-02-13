#include "pch.h"
#include "KeyManager.h"

void KeyInit()
{
	// 키 배열 초기화
	for (int i = 0; i < (int)KEY::END; i++)
	{
		g_KeyInfo[i] = stKeyInfo{ KEY_STATE::NONE, false };
	}
}

// 프레임 당 키 상태 체크
void KeyUpdate()
{
	for (int i = 0; i < (int)KEY::END; i++)
	{
		// 눌린 상태
		if (GetAsyncKeyState(g_arrVKKey[i]))
		{
			if (g_KeyInfo[i].m_bPrevPush)
			{
				// 이전에도 눌려있었다.
				g_KeyInfo[i].m_eState = KEY_STATE::HOLD;
			}
			else
			{
				// 이전에 눌려있지 않았다.
				g_KeyInfo[i].m_eState = KEY_STATE::TAP;
			}
			g_KeyInfo[i].m_bPrevPush = true;
		}
		else // 키가 눌려있지 않다.
		{
			if (g_KeyInfo[i].m_bPrevPush)
			{
				// 이전에는 눌려있었다.
				g_KeyInfo[i].m_eState = KEY_STATE::AWAY;
			}
			else
			{
				// 이전에도 안눌려있었다.
				g_KeyInfo[i].m_eState = KEY_STATE::NONE;
			}
			g_KeyInfo[i].m_bPrevPush = false;
		}
	}
}

KEY_STATE GetKeyState(KEY key)
{
	return g_KeyInfo[(int)key].m_eState;
}