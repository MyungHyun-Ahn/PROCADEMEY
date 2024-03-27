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

	// 미사일 정보 - m_MissileInfos는 할당해제하면 안된다.
	// 외부 map 에 저장한 것을 그냥 가져다 쓸 뿐
	int						m_iMissileCount = 0;
	stMissileInfo			*m_MissileInfos = nullptr;

	// 미사일 쿨타임 정보 - 이것은 Enemy 가 동적할당 해제해야 한다.
	unsigned int			*m_arrMissilePrevTime;

	// 이동 경로
	int						m_iMoveCount;
	int						m_iCurMoveIndex;
	double					m_dCurMoveIndex;
	stPos					*m_arrMovePos = nullptr; // 이것 또한 여기서 할당해제하지 않는다.
};

