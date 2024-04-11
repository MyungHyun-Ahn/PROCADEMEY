#pragma once

struct Pos
{
	int y;
	int x;
};

struct Room
{
	bool isVisited = false;
	int counter = 0;
	// 0, 1, 2, .. 번 룸 간의 최소 거리
	std::map<int, double> distances;
	// positions[0] - startPos, position[1] - destPos
	std::map<int, std::pair<Pos, Pos>> positions;
	std::vector<Pos> boundarys;

	std::vector<int> toFroms;
};

class CellularAutomata
{
public:
	CellularAutomata(int heigth, int width, int smoothCount, int randomFillPercent, time_t seed, bool useSeed);
	~CellularAutomata();

	void		Generate();

private:
	// 사용하는 맵의 형식에 따라 재정의해야 한다.
	void				SaveMap();
	void				RandomFillMap();
	void				SmoothMap();
	int					GetSurrouningWallCount(int GridY, int GridX);
	void				FindBoundarys(int y, int x, int &counter);
	void				CalDistances();
	void				Prim();
	void				Merge();
	void				Bresenham(int startY, int startX, int destY, int destX);
	inline double 		CalEuclidean(Pos &startPos, Pos &destPos)
	{
		return	sqrt(pow(startPos.y - destPos.y, 2) + pow(startPos.x - destPos.x, 2));
	}
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