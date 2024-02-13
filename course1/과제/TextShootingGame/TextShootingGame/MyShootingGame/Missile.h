#pragma once

// �̻��� ���� ����ü
struct stMissileInfo
{
	// MISSILE ����
	char				m_szFileName[MAX_FILE_NAME]; // ���� �̸�
	stPos				m_iStartPos;				// �̻��� ���� ��� ��ǥ
	unsigned int		m_iFirstTime;				// �̻��� ó�� �߻� Ÿ�̹�
	unsigned int		m_iCooltime;				// �̻��� ��Ÿ��
	int					m_iDamage;					// �̻��� ������
	char				m_chShape;
	int					m_iLifetime;
	int					m_iMissileMaxMoveIndex;		// �̻��� ���� �� �ִ� �ε���
	stPos				*m_iMissileMoves;			// �̻��� ������
};

struct stMissile
{
	// �̻��� Ȱ��ȭ ����
	bool m_bIsActive;

	// ���� ������
	bool m_bIsEnemy;

	// �̻��� ������
	int m_iDamage;

	// ���� ��ǥ
	stPos m_stPos;

	// �̻��� ���
	char m_chShape;

	// �̻��� ������ Ÿ��
	// �� ������ ������ �� �ִ°�
	int m_iCurLife = 0;
	int m_iMaxLife = INT32_MAX;

	// ���� ���� - �پ��� ���� ������ ����
	// Missile���� ���� �Ҵ� �����ؼ��� �ȵȴ�.
	int m_iDirSize;
	int m_iDirIndex = 0;
	stPos *m_arrDir = nullptr;

	// �÷��̾� ���� ���� ���� �����
	stPos m_DirBak;
};


// Enemy �̻��� ����
void MissileEnemyCreate(const stMissileInfo &info, const stPos &curPos);

// Player Defualt �̻��� ����
// curDir�� ���� ����
void MissilePlayerDefualtCreate(const stPos &curDir);

// �̻��� Logic Update
void MissileUpdate(void);

// �̻��� ������
void MissileMove(void);

// �̻��� �浹 ó��
void MissileCollision(void);

// �̻��� Render
void MissileRender(void);