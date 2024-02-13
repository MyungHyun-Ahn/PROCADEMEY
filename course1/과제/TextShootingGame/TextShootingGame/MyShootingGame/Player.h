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

	// 플레이어 미사일 쿨타임 - Space
	int m_iCoolTime;
	unsigned __int64 m_iPrevAttackTime = 0;


	// 플레이어 스킬 현황
	// TODO:
};

extern stPlayer g_stPlayer;