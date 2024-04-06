#pragma once
class Bresenham
{
public:
	Bresenham();
	~Bresenham();

	void Search(HWND hWnd, HDC hdc);
	bool CalBresenham(int startY, int startX, int destY, int destX);

private:
	HWND m_hWnd;
	HDC m_hdc;
	std::vector<Node *> m_vNodes;
};

