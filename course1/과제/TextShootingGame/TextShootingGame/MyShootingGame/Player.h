#pragma once

struct stPlayer
{
	// �÷��̾� ���� HP
	int m_iCurHp;

	// �÷��̾� �ִ� HP
	int m_iMaxHp;

	// �÷��̾� ��ǥ
	stPos m_stPos;

	// �÷��̾� ���ݷ�
	int m_iDamage;

	// �÷��̾� �̻��� ��Ÿ�� - Space
	int m_iCoolTime;
	unsigned __int64 m_iPrevAttackTime = 0;


	// �÷��̾� ��ų ��Ȳ
	// TODO:
};

extern stPlayer g_stPlayer;