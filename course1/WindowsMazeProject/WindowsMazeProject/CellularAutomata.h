#pragma once
class CellularAutomata
{
public:
	CellularAutomata(int heigth, int width, int smoothCount, int randomFillPercent, time_t seed, bool useSeed);
	~CellularAutomata();

	void Generate();

private:
	// 사용하는 맵의 형식에 따라 재정의해야 한다.
	void SaveMap();
	void RandomFillMap();
	void SmoothMap();
	int GetSurrouningWallCount(int GridY, int GridX);

private:
	int		m_iHeigth;
	int		m_iWidth;

	int		m_iRandomFillPercent;
	time_t	m_iSeed;
	bool	m_bUseRandomSeed;

	int		m_iSmoothCount;
	int		*m_arrMap;
};

