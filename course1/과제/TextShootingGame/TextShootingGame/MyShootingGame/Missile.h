#pragma once

struct stMissile
{
	bool m_bIsActive;

	// ���� ������
	bool m_bIsEnemy;

	wchar_t m_chShape;

	int m_iDamage;

	// ���� ��ǥ
	stPos m_stPos;

	// ����
	stPos m_stDir;
};

extern stMissile g_arrMissile[dfMAX_MISSILE_COUNT];

void MissileInit();

void MissileCreate(bool isEnemy, int damage, const stPos &&startPos, const stPos &&dir);

wchar_t MissileGetShape(stPos &dir);

void MissileUpdate();

void MissileMove();

void MissileDraw();

void MissileCollision();