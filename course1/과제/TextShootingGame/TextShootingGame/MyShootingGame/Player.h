#pragma once

struct stPlayer
{
	// 플레이어 현재 HP
	int m_iCurHp;

	// 플레이어 최대 HP
	int m_iMaxHp;

	// 플레이어 좌표
	stPos m_stPos;

	// 플레이어 공격력
	int m_iDamage;

	// 플레이어 기본 공격 쿨타임
	int m_iCoolTime;
	unsigned __int64 m_iPrevAttackTime = 0;

	// 플레이어 스킬 현황
	// TODO:
};

extern stPlayer g_stPlayer;

// Player 초기화 함수
void PlayerInit(int iY, int iX, int iMaxHp);

// 플레이어 로직 업데이트
void PlayerUpdate();

// 플레이어 움직임 처리
void PlayerMove();

// Player 공격
void PlayerAttack();

// Player 피격 처리
void PlayerGetDamage(int damage);

// Player 그리기
void PlayerDraw();