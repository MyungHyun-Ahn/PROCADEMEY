#pragma once

struct stMissile
{
	bool m_bIsActive;

	// ���� ������
	bool m_bIsEnemy;

	wchar_t m_chShape;

	// ���� ��ǥ
	stPos m_stPos;

	// ����
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