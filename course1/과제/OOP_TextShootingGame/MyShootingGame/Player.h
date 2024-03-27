#pragma once
class Player : public BaseObject
{
public:
	Player();
	~Player();
	
	void Update() override;
	void GetDamage(int damage);

	void Render() override;
	inline void CooltimeReset() { m_iPrevAttackTime = 0; }

private:
	void Init();
	void Move();
	void Attack();

private:
	// 플레이어 HP
	int m_iCurHp;
	int m_iMaxHp;

	// 플레이어 공격력
	int m_iDamage;

	// 플레이어 스피드
	int m_iSpeed;

	// 플레이어 좌표 - Speed 조절을 위해
	stPosD m_stPosD;

	// 플레이어 미사일 쿨타임 - Space
	unsigned int m_iCoolTime;
	unsigned int m_iPrevAttackTime = 0;
};

