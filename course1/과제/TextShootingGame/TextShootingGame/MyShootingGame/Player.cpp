#include "pch.h"
#include "Player.h"
#include "Console.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Missile.h"

void PlayerInit(void)
{
	g_stPlayer.m_iCurHp = dfPLAYER_HP;
	g_stPlayer.m_iMaxHp = dfPLAYER_HP;
	g_stPlayer.m_iDamage = dfPLAYER_DAMAGE;

	g_stPlayer.m_stPos = stPos{ dfPLAYER_START_POS_Y, dfPLAYER_START_POS_X };

	PlayerCooltimeReset();
	g_stPlayer.m_iCoolTime = dfPLAYER_ATTACK_COOLTIME;
}

void PlayerUpdate(void)
{
	PlayerMove();
	PlayerAttack();
}

void PlayerMove(void)
{
	if (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP))
	{
		g_stPlayer.m_stPos.m_iY -= 1;
	}

	if (KEY_TAP(KEY::DOWN) || KEY_HOLD(KEY::DOWN))
	{
		g_stPlayer.m_stPos.m_iY += 1;
	}

	if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT))
	{
		g_stPlayer.m_stPos.m_iX -= 1;
	}

	if (KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
	{
		g_stPlayer.m_stPos.m_iX += 1;
	}

	// ��ǥ ��ȿ�� üũ
	g_stPlayer.m_stPos.m_iY = max(g_stPlayer.m_stPos.m_iY, 0);
	g_stPlayer.m_stPos.m_iY = min(g_stPlayer.m_stPos.m_iY, dfSCREEN_HEIGHT - 1);
	g_stPlayer.m_stPos.m_iX = max(g_stPlayer.m_stPos.m_iX, 0);
	g_stPlayer.m_stPos.m_iX = min(g_stPlayer.m_stPos.m_iX, dfSCREEN_WIDTH - 2);
}

void PlayerAttack(void)
{
	// SPACE�� ������ �ʾҴٸ� return
	if (!(KEY_TAP(KEY::SPACE) || KEY_HOLD(KEY::SPACE)))
		return;

	// ���� �߻��� �� ���� ��Ÿ���̸� return
	if (!TimerCalCoolTime(g_stPlayer.m_iPrevAttackTime, g_stPlayer.m_iCoolTime))
		return;

	stPos mDir{ 0,0 };

	// �÷��̾ �ֽ��ϰ� �ִ� �������� �߻�
	if (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP))
	{
		mDir.m_iY -= 1;
	}

	if (KEY_TAP(KEY::DOWN) || KEY_HOLD(KEY::DOWN))
	{
		mDir.m_iY += 1;
	}

	if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT))
	{
		mDir.m_iX -= 1;
	}

	if (KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
	{
		mDir.m_iX += 1;
	}

	// ���� �ƹ� Ű�� ������ ���� �ʾҴٸ� ���� �߻�
	if (mDir.m_iY == 0 && mDir.m_iX == 0)
	{
		mDir.m_iY -= 1;
	}

	MissilePlayerDefualtCreate(mDir);
}

void PlayerGetDamage(int damage)
{
	g_stPlayer.m_iCurHp -= damage;

	if (g_stPlayer.m_iCurHp <= 0)
	{
		// ���� ����
		// �̰����� ���� ������ �Ǵ�
		g_bIsGameOver = true;
	}
}

void PlayerRender(void)
{
	ConsoleSpriteDraw(g_stPlayer.m_stPos, dfPLAYER_SHAPE);
}
