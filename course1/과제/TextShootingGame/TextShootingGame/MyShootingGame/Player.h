#pragma once

struct stPlayer
{
	// �÷��̾� ���� HP
	int m_iCurHp;

	// �÷��̾� �ִ� HP
	int m_iMaxHp;

	// �÷��̾� ��ǥ
	stPos m_stPos;

	// �÷��̾� ��ų ��Ȳ
	// TODO:
};

// Player �ʱ�ȭ �Լ�
void PlayerInit(int iY, int iX, int iMaxHp);

// �÷��̾� ���� ������Ʈ
void PlayerUpdate();

// �÷��̾� ������ ó��
void PlayerMove();

// Player ����
void PlayerAttack();

// Player �׸���
void PlayerDraw();