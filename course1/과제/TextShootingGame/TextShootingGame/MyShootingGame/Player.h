#pragma once

struct stPlayer
{
	// 플레이어 HP
	int m_iCurHp;
	int m_iMaxHp;

	// 플레이어 공격력
	int m_iDamage;

	// 플레이어 스피드
	int m_iSpeed;

	// 플레이어 좌표
	stPosD m_stPosD;

	// 플레이어 미사일 쿨타임 - Space
	unsigned int m_iCoolTime;
	unsigned int m_iPrevAttackTime = 0;
};

// Player 초기화
void PlayerInit(void);
inline void PlayerCooltimeReset(void)
{
	g_stPlayer.m_iPrevAttackTime = 0;
}

// Player Logic Update
void PlayerUpdate(void);

// Player 움직임
void PlayerMove(void);

// Player 미사일 발사
void PlayerAttack(void);

// Player Get Damage
void PlayerGetDamage(int damage);

// Player Render
void PlayerRender(void);