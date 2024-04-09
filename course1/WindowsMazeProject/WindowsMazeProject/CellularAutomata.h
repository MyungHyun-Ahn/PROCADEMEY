#pragma once

struct Room
{
	int counter;
	std::vector<std::pair<int, int>> boundarys;
};

class CellularAutomata
{
public:
	CellularAutomata(int heigth, int width, int smoothCount, int randomFillPercent, time_t seed, bool useSeed);
	~CellularAutomata();

	void		Generate();

private:
	// ����ϴ� ���� ���Ŀ� ���� �������ؾ� �Ѵ�.
	void		SaveMap();
	void		RandomFillMap();
	void		SmoothMap();
	int			GetSurrouningWallCount(int GridY, int GridX);
	void		FindBoundarys(int y, int x, std::vector<std::pair<int, int>> &component, int &counter);
private:
	int		m_iHeigth;
	int		m_iWidth;

	int		m_iRandomFillPercent;
	time_t	m_iSeed;
	bool	m_bUseRandomSeed;

	int		m_iSmoothCount;
	int		*m_arrMap;

	std::vector<Room> m_arrRooms;
};