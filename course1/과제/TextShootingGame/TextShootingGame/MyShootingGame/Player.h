#pragma once

struct stPlayer
{
	// �÷��̾� HP
	int m_iCurHp;
	int m_iMaxHp;

	// �÷��̾� ���ݷ�
	int m_iDamage;

	// �÷��̾� ���ǵ�
	int m_iSpeed;

	// �÷��̾� ��ǥ
	stPosD m_stPosD;

	// �÷��̾� �̻��� ��Ÿ�� - Space
	unsigned int m_iCoolTime;
	unsigned int m_iPrevAttackTime = 0;
};

// Player �ʱ�ȭ
void PlayerInit(void);
inline void PlayerCooltimeReset(void)
{
	g_stPlayer.m_iPrevAttackTime = 0;
}

// Player Logic Update
void PlayerUpdate(void);

// Player ������
void PlayerMove(void);

// Player �̻��� �߻�
void PlayerAttack(void);

// Player Get Damage
void PlayerGetDamage(int damage);

// Player Render
void PlayerRender(void);