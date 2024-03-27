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
	// �÷��̾� HP
	int m_iCurHp;
	int m_iMaxHp;

	// �÷��̾� ���ݷ�
	int m_iDamage;

	// �÷��̾� ���ǵ�
	int m_iSpeed;

	// �÷��̾� ��ǥ - Speed ������ ����
	stPosD m_stPosD;

	// �÷��̾� �̻��� ��Ÿ�� - Space
	unsigned int m_iCoolTime;
	unsigned int m_iPrevAttackTime = 0;
};

