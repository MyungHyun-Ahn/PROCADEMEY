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
	// Ű �迭 �ʱ�ȭ
	for (int i = 0; i < (int)KEY::END; i++)
	{
		m_KeyInfo[i] = stKeyInfo{ KEY_STATE::NONE, false };
	}
}

// ������ �� Ű ���� üũ
void KeyManager::Update()
{
	for (int i = 0; i < (int)KEY::END; i++)
	{
		// ���� ����
		if (GetAsyncKeyState(g_arrVKKey[i]))
		{
			if (m_KeyInfo[i].m_bPrevPush)
			{
				// �������� �����־���.
				m_KeyInfo[i].m_eState = KEY_STATE::HOLD;
			}
			else
			{
				// ������ �������� �ʾҴ�.
				m_KeyInfo[i].m_eState = KEY_STATE::TAP;
			}
			m_KeyInfo[i].m_bPrevPush = true;
		}
		else // Ű�� �������� �ʴ�.
		{
			if (m_KeyInfo[i].m_bPrevPush)
			{
				// �������� �����־���.
				m_KeyInfo[i].m_eState = KEY_STATE::AWAY;
			}
			else
			{
				// �������� �ȴ����־���.
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