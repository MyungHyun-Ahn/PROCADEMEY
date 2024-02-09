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

	// �÷��̾� �⺻ ���� ��Ÿ��
	int m_iCoolTime;
	unsigned __int64 m_iPrevAttackTime = 0;

	// �÷��̾� ��ų ��Ȳ
	// TODO:
};

extern stPlayer g_stPlayer;

// Player �ʱ�ȭ �Լ�
void PlayerInit(int iY, int iX, int iMaxHp);

// �÷��̾� ���� ������Ʈ
void PlayerUpdate();

// �÷��̾� ������ ó��
void PlayerMove();

// Player ����
void PlayerAttack();

// Player �ǰ� ó��
void PlayerGetDamage(int damage);

// Player �׸���
void PlayerDraw();