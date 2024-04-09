#include "pch.h"
#include "Bresenham.h"

#define debug

Bresenham::Bresenham()
{
	Node *curNode = g_arrNodes[destY][destX];
	while (true)
	{
		m_vNodes.push_back(curNode);
		if (curNode->parent == NULL)
			break;

		curNode = curNode->parent;
	}

	std::reverse(m_vNodes.begin(), m_vNodes.end());

#ifdef debug

	std::cout << "route position" << std::endl;

	for (auto node : m_vNodes)
	{
		std::cout << node->y << " " << node->x << std::endl;
	}

#endif
}

Bresenham::~Bresenham()
{
}

void Bresenham::Search(HWND hWnd, HDC hdc)
{
	m_hWnd = hWnd;
	m_hdc = hdc;

	int s = 0;
	int d = s + 2;
	Node *sNode = m_vNodes[s];
	Node *dNode = nullptr;
	while (d < m_vNodes.size())
	{
		dNode = m_vNodes[d];
		std::cout << "sNode : " << sNode->y << " " << sNode->x << std::endl;
		std::cout << "dNode : " << dNode->y << " " << dNode->x << std::endl;

		int startY = sNode->y;
		int startX = sNode->x;

		int destY = dNode->y;
		int destX = dNode->x;

		if (CalBresenham(startY, startX, destY, destX))
		{
			sNode->bresenhamParent = dNode;
			d++; // d를 다음 노드로
		}
		else
		{
			sNode->bresenhamParent = m_vNodes[d - 1];
			sNode = m_vNodes[d - 1];
			s = d - 1;
			d = s + 2;

			// 충돌이 발생했는데 d가 vector의 사이즈를 초과한 경우
			if (d >= m_vNodes.size())
			{
				sNode->bresenhamParent = m_vNodes.back();
			}

		}

		InvalidateRect(m_hWnd, nullptr, false);
		UpdateWindow(m_hWnd);
	}
}

bool Bresenham::CalBresenham(int startY, int startX, int destY, int destX)
{
	// start와 dest 반전 처리
	bool reversed = false;
	if (abs(destX - startX) < abs(destY - startY)) {
		std::swap(startX, startY);
		std::swap(destX, destY);
		reversed = true;
	}

	// 시작점이 목적지보다 오른쪽에 있는 경우 교환
	if (startX > destX) {
		std::swap(startX, destX);
		std::swap(startY, destY);
	}

	int dx = destX - startX;
	int dy = abs(destY - startY);
	int err = dx / 2;
	int ystep = (startY < destY) ? 1 : -1;

	for (int x = startX, y = startY; x <= destX; x++) {
		if (reversed) {
			std::cout << "브레젠험 : " << x << " " << y << std::endl;

			// 충돌 판단
			if (g_Tile[x][y] == TILE_TYPE::OBSTACLE)
			{
				std::cout << "충돌 : " << x << " " << y << std::endl;
				return false;
			}
		}
		else {
			std::cout << "브레젠험 : " << y << " " << x << std::endl;

			// 충돌 판단
			if (g_Tile[y][x] == TILE_TYPE::OBSTACLE)
			{
				std::cout << "충돌 : " << y << " " << x << std::endl;
				return false;
			}
		}

		err -= dy;
		if (err < 0) {
			y += ystep;
			err += dx;
		}
	}

	return true;
}