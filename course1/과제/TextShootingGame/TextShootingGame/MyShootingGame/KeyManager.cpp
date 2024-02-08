#include "pch.h"
#include "KeyManager.h"

int g_arrVKKey[(int)KEY::END] =
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	'Q', 'W', 'E', 'R',
	'D', 'F',

	VK_CONTROL,
	VK_SPACE,
	VK_ESCAPE
};

stKeyInfo g_KeyInfo[(int)KEY::END];

void KeyInit()
{
	// Ű �迭 �ʱ�ȭ
	for (int i = 0; i < (int)KEY::END; i++)
	{
		g_KeyInfo[i] = stKeyInfo{ KEY_STATE::NONE, false };
	}
}

// ������ �� Ű ���� üũ
void KeyUpdate()
{
	for (int i = 0; i < (int)KEY::END; i++)
	{
		// ���� ����
		if (GetAsyncKeyState(g_arrVKKey[i]))
		{
			if (g_KeyInfo[i].m_bPrevPush)
			{
				// �������� �����־���.
				g_KeyInfo[i].m_eState = KEY_STATE::HOLD;
			}
			else
			{
				// ������ �������� �ʾҴ�.
				g_KeyInfo[i].m_eState = KEY_STATE::TAP;
			}
			g_KeyInfo[i].m_bPrevPush = true;
		}
		else // Ű�� �������� �ʴ�.
		{
			if (g_KeyInfo[i].m_bPrevPush)
			{
				// �������� �����־���.
				g_KeyInfo[i].m_eState = KEY_STATE::AWAY;
			}
			else
			{
				// �������� �ȴ����־���.
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