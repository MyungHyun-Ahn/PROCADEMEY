#include "pch.h"
#include "Player.h"
#include "Console.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "Missile.h"

void PlayerInit(void)
{
	g_bIsGameOver = false;
	g_stPlayer.m_iCurHp = dfPLAYER_HP;
	g_stPlayer.m_iMaxHp = dfPLAYER_HP;
	g_stPlayer.m_iDamage = dfPLAYER_DAMAGE;
	g_stPlayer.m_iSpeed = dfPLAYER_SPEED;
	g_stPlayer.m_stPosD = stPosD{ dfPLAYER_START_POS_Y, dfPLAYER_START_POS_X };

	PlayerCooltimeReset();
	g_stPlayer.m_iCoolTime = dfPLAYER_ATTACK_COOLTIME;
}

void PlayerUpdate(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	PlayerMove();
	PlayerAttack();
}

void PlayerMove(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	if (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP))
	{
		g_stPlayer.m_stPosD.m_dY -= g_stPlayer.m_iSpeed * dfFIXED_DELTATIME;
	}

	if (KEY_TAP(KEY::DOWN) || KEY_HOLD(KEY::DOWN))
	{
		g_stPlayer.m_stPosD.m_dY += g_stPlayer.m_iSpeed * dfFIXED_DELTATIME;
	}

	if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT))
	{
		g_stPlayer.m_stPosD.m_dX -= g_stPlayer.m_iSpeed * dfFIXED_DELTATIME;
	}

	if (KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
	{
		g_stPlayer.m_stPosD.m_dX += g_stPlayer.m_iSpeed * dfFIXED_DELTATIME;
	}

	// ��ǥ ��ȿ�� üũ
	g_stPlayer.m_stPosD.m_dY = max(g_stPlayer.m_stPosD.m_dY, 0);
	g_stPlayer.m_stPosD.m_dY = min(g_stPlayer.m_stPosD.m_dY, dfSCREEN_HEIGHT - 1);
	g_stPlayer.m_stPosD.m_dX = max(g_stPlayer.m_stPosD.m_dX, 0);
	g_stPlayer.m_stPosD.m_dX = min(g_stPlayer.m_stPosD.m_dX, dfSCREEN_WIDTH - 2);
}

void PlayerAttack(void)
{
	PROFILE_BEGIN(__WFUNC__, 0);
	// SPACE�� ������ �ʾҴٸ� return
	if (!(KEY_TAP(KEY::SPACE) || KEY_HOLD(KEY::SPACE)))
		return;

	// ���� �߻��� �� ���� ��Ÿ���̸� return
	if (!TimerCalCoolTime(g_stPlayer.m_iPrevAttackTime, g_stPlayer.m_iCoolTime))
		return;

	stPos mDir{ -1,0 };

#pragma region SHOOTING_DIR
	//// �÷��̾ �ֽ��ϰ� �ִ� �������� �߻�
	//if (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP))
	//{
	//	mDir.m_iY -= 1;
	//}

	//if (KEY_TAP(KEY::DOWN) || KEY_HOLD(KEY::DOWN))
	//{
	//	mDir.m_iY += 1;
	//}

	//if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT))
	//{
	//	mDir.m_iX -= 1;
	//}

	//if (KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
	//{
	//	mDir.m_iX += 1;
	//}

	//// ���� �ƹ� Ű�� ������ ���� �ʾҴٸ� ���� �߻�
	//if (mDir.m_iY == 0 && mDir.m_iX == 0)
	//{
	//	mDir.m_iY -= 1;
	//}
#pragma endregion

	MissilePlayerDefualtCreate(mDir);
}

void PlayerGetDamage(int damage)
{
	PROFILE_BEGIN(__WFUNC__, 0);
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
	PROFILE_BEGIN(__WFUNC__, 0);
	ConsoleSpriteDraw(POS_DTOI(g_stPlayer.m_stPosD), dfPLAYER_SHAPE);

	char playerHpUi[20];
	sprintf_s(playerHpUi, "HP : %02d / %02d", g_stPlayer.m_iCurHp, g_stPlayer.m_iMaxHp);

	for (int i = 0; playerHpUi[i] != '\0'; i++)
	{
		ConsoleSpriteDraw({0, i}, playerHpUi[i]);
	}
}
