#pragma once
class CellularAutomata
{
public:
	CellularAutomata(int heigth, int width, int smoothCount, int randomFillPercent, time_t seed, bool useSeed);
	~CellularAutomata();

	void Generate();

private:
	// ����ϴ� ���� ���Ŀ� ���� �������ؾ� �Ѵ�.
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

