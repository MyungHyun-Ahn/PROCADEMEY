#pragma once

// Stage ���� ����ü
struct stStageInfo
{
	char	*m_szFileName;		// Stage ���� �̸�
	int		m_iEnemyCount;		// �ʿ� �����ϴ� Enemy ��
	int		m_iEnemyTypeCount;	// Enemy ���� ��
	char	*m_arrEnemys;		// Enemy ���� �迭
};

class GameScene : public Scene
{
public:
	void Update() override;
	void Render() override;

	void Register(char *buffer) override;
};

