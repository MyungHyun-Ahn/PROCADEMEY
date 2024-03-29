#pragma once
// �̻��� ���� ����ü
struct stMissileInfo
{
	// MISSILE ����
	char				m_szFileName[MAX_FILE_NAME];	// ���� �̸�
	stPos				m_iStartPos;					// �̻��� ���� ��� ��ǥ
	unsigned int		m_iFirstTime;					// �̻��� ó�� �߻� Ÿ�̹�
	unsigned int		m_iCooltime;					// �̻��� ��Ÿ��
	int					m_iDamage;						// �̻��� ������
	int					m_iSpeed;						// �̻��� �ӵ�
	char				m_chShape;						// �̻��� ���
	int					m_iLifetime;					// �̻��� �����ֱ�
	int					m_iMissileMaxMoveIndex;			// �̻��� ���� �� �ִ� �ε���
	stPos				*m_MissileMoves;				// �̻��� ������
};

class Missile : public BaseObject
{
public:
	// ���� ��
	Missile(bool isEnemy, const stPos &startPos, const stMissileInfo &info);

	// Player ��
	Missile(bool isEnemy, const stPos &curDir, const stPos &startPos);
	~Missile();

	void Update() override;
	void Render() override;

	void Collision(BaseObject *obj);

	inline bool isEnemyMissile() { return m_bIsEnemy; }

private:
	void Move();
	bool CheckCollision(stPos &targetPos);

private:
	// ���� ������
	bool				m_bIsEnemy;

	// �̻��� ������
	int					m_iDamage;
	int					m_iSpeed;

	// �̻��� ���
	char				m_chShape;

	// �̻��� ������ Ÿ��
	// �� ������ ������ �� �ִ°�
	int					m_iCurLife = 0;
	int					m_iMaxLife = INT32_MAX;

	// ���� ���� - �پ��� ���� ������ ����
	// Missile���� ���� �Ҵ� �����ؼ��� �ȵȴ�.
	int					m_iDirSize;
	int					m_iDirIndex = 0;
	double				m_dDirIndex = 0; // double index : deltaTime ���ϴ� �뵵
	stPos *m_arrDir = nullptr;

	// �÷��̾� ���� ���� ���� �����
	stPos				m_DirBak;
};

