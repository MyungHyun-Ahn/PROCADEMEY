#include "pch.h"
#include "GameSetting.h"
#include "BaseObject.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "ConsoleManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "ObjectManager.h"

Player::Player() : BaseObject(ObjectType::Player, { 0, 0 })
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	m_iCurHp = dfPLAYER_HP;
	m_iMaxHp = dfPLAYER_HP;
	m_iDamage = dfPLAYER_DAMAGE;
	m_iSpeed = dfPLAYER_SPEED;
	m_stPosD = stPosD{ dfPLAYER_START_POS_Y, dfPLAYER_START_POS_X };
	m_iCoolTime = dfPLAYER_ATTACK_COOLTIME;
}

void Player::Update()
{
	Move();
	Attack();
}

void Player::Move()
{
	if (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP))
	{
		m_stPosD.m_dY -= m_iSpeed * dfFIXED_DELTATIME;
	}

	if (KEY_TAP(KEY::DOWN) || KEY_HOLD(KEY::DOWN))
	{
		m_stPosD.m_dY += m_iSpeed * dfFIXED_DELTATIME;
	}

	if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT))
	{
		m_stPosD.m_dX -= m_iSpeed * dfFIXED_DELTATIME;
	}

	if (KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
	{
		m_stPosD.m_dX += m_iSpeed * dfFIXED_DELTATIME;
	}

	// 좌표 유효성 체크
	m_stPosD.m_dY = max(m_stPosD.m_dY, 0);
	m_stPosD.m_dY = min(m_stPosD.m_dY, dfSCREEN_HEIGHT - 1);
	m_stPosD.m_dX = max(m_stPosD.m_dX, 0);
	m_stPosD.m_dX = min(m_stPosD.m_dX, dfSCREEN_WIDTH - 2);

	m_Pos = POS_DTOI(m_stPosD);
}

void Player::Attack()
{
	// SPACE가 눌리지 않았다면 return
	if (!(KEY_TAP(KEY::SPACE) || KEY_HOLD(KEY::SPACE)))
		return;

	// 아직 발사할 수 없는 쿨타임이면 return
 	if (!g_TimeMgr->CalCoolTime(m_iPrevAttackTime, m_iCoolTime))
		return;

    stPos mDir{ -1,0 };

	// 미사일 생성 : TODO
	g_ObjMgr->CreatePlayerMissile(mDir, m_Pos);
}

void Player::GetDamage(int damage)
{
	m_iCurHp -= damage;

	if (m_iCurHp <= 0)
	{
		// 게임 오버
		// 이것으로 게임 오버를 판단
		m_bIsActive = false;
	}
}

void Player::Render()
{
	g_ConsoleMgr->SpriteDraw(m_Pos, dfPLAYER_SHAPE);
}
