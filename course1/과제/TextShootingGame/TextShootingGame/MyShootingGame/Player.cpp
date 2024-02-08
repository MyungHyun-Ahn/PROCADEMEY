#include "pch.h"
#include "Player.h"
#include "Console.h"
#include "KeyManager.h"
#include "Missile.h"

static stPlayer g_stPlayer;

void PlayerInit(int iY, int iX, int iMaxHp)
{
	g_stPlayer.m_stPos.m_iY = iY;
	g_stPlayer.m_stPos.m_iX = iX;
	g_stPlayer.m_iMaxHp = iMaxHp;

	g_stPlayer.m_iCurHp = iMaxHp;
}

void PlayerUpdate()
{
	PlayerMove();
	PlayerAttack();
}

void PlayerMove()
{
	if (KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::LEFT))
	{
		g_stPlayer.m_stPos.m_iX -= 1;
	}

	if (KEY_HOLD(KEY::RIGHT) || KEY_TAP(KEY::RIGHT))
	{
		g_stPlayer.m_stPos.m_iX += 1;
	}

	if (KEY_HOLD(KEY::UP) || KEY_TAP(KEY::UP))
	{
		g_stPlayer.m_stPos.m_iY -= 1;
	}

	if (KEY_HOLD(KEY::DOWN) || KEY_TAP(KEY::DOWN))
	{
		g_stPlayer.m_stPos.m_iY += 1;
	}

	g_stPlayer.m_stPos.m_iX = max(g_stPlayer.m_stPos.m_iX, 0);
	g_stPlayer.m_stPos.m_iX = min(g_stPlayer.m_stPos.m_iX, dfSCREEN_WIDTH - 2);
	g_stPlayer.m_stPos.m_iY = max(g_stPlayer.m_stPos.m_iY, 0);
	g_stPlayer.m_stPos.m_iY = min(g_stPlayer.m_stPos.m_iY, dfSCREEN_HEIGHT - 2);
}

void PlayerAttack()
{
	if (!(KEY_HOLD(KEY::SPACE) || KEY_TAP(KEY::SPACE)))
		return;
	

 	stPos nextDir{ 0,0 };

	if (KEY_HOLD(KEY::LEFT) || KEY_TAP(KEY::LEFT))
	{
		nextDir.m_iX -= 1;
	}

	if (KEY_HOLD(KEY::RIGHT) || KEY_TAP(KEY::RIGHT))
	{
		nextDir.m_iX += 1;
	}

	if (KEY_HOLD(KEY::UP) || KEY_TAP(KEY::UP))
	{
		nextDir.m_iY -= 1;
	}

	if (KEY_HOLD(KEY::DOWN) || KEY_TAP(KEY::DOWN))
	{
		nextDir.m_iY += 1;
	}

	// 가만히 있으면 위로 쏘기
	if (nextDir.m_iY == 0 && nextDir.m_iX == 0)
		nextDir.m_iY -= 1;

	MissileCreate(false, g_stPlayer.m_stPos.m_iY + nextDir.m_iY, g_stPlayer.m_stPos.m_iX + nextDir.m_iX, std::move(nextDir));
}

void PlayerDraw()
{
	if (g_stPlayer.m_iCurHp <= g_stPlayer.m_iMaxHp)
		ConsoleSpriteDraw(g_stPlayer.m_stPos, L'#');
}
