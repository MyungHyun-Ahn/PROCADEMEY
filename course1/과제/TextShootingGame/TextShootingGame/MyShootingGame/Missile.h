#pragma once

struct stMissile
{
	bool m_bIsActive;

	// 적이 쐈는지
	bool m_bIsEnemy;

	wchar_t m_chShape;

	// 현재 좌표
	stPos m_stPos;

	// 방향
	stPos m_stDir;
};

extern stMissile g_arrMissile[dfMAX_MISSILE_COUNT];

void MissileInit();

void MissileCreate(bool isEnemy, int posY, int posX, const stPos &&dir);

wchar_t MissileGetShape(stPos &dir);

void MissileUpdate();

void MissileMove();

void MissileDraw();

void MissileCollison();