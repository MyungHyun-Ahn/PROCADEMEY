#pragma once

struct stMissileInfo;

struct stEnemyInfo
{
	// Shape
	char					m_chShape;

	// hp
	int						m_iHp;
	int						m_iSpeed;

	int						m_iMissileCount;
	stMissileInfo			*m_MissileInfos;

	int						m_iMovesCount;
	stPos *m_Moves;
};

class Enemy : public BaseObject
{
public:
	Enemy(stPos pos, stEnemyInfo *info);
	~Enemy();

	void Init(stEnemyInfo *info);
	void Release();
	void Update() override;
	void GetDamage(int damage);
	void Render() override;

private:
	void Move();
	void Attack();

private:
	// SHAPE
	char					m_chShape;

	// STATS
	int						m_iCurHp;
	int						m_iMaxHp;
	int						m_iSpeed;

	// �̻��� ���� - m_MissileInfos�� �Ҵ������ϸ� �ȵȴ�.
	// �ܺ� map �� ������ ���� �׳� ������ �� ��
	int						m_iMissileCount = 0;
	stMissileInfo			*m_MissileInfos = nullptr;

	// �̻��� ��Ÿ�� ���� - �̰��� Enemy �� �����Ҵ� �����ؾ� �Ѵ�.
	unsigned int			*m_arrMissilePrevTime;

	// �̵� ���
	int						m_iMoveCount;
	int						m_iCurMoveIndex;
	double					m_dCurMoveIndex;
	stPos					*m_arrMovePos = nullptr; // �̰� ���� ���⼭ �Ҵ��������� �ʴ´�.
};

