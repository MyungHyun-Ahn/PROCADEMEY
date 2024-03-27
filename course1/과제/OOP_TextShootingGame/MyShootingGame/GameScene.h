#pragma once

// Stage 정보 구조체
struct stStageInfo
{
	char	*m_szFileName;		// Stage 파일 이름
	int		m_iEnemyCount;		// 맵에 존재하는 Enemy 수
	int		m_iEnemyTypeCount;	// Enemy 종류 수
	char	*m_arrEnemys;		// Enemy 저장 배열
};

class GameScene : public Scene
{
public:
	void Update() override;
	void Render() override;

	void Register(char *buffer) override;
};

