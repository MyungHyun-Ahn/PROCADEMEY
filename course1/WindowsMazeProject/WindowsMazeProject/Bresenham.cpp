#include "pch.h"
#include "Bresenham.h"

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

		int startY = sNode->y;
		int startX = sNode->x;

		int destY = dNode->y;
		int destX = dNode->x;

		// false - �浹�� �߻�
		if (!CalBresenham(startY, startX, destY, destX))
		{
			// sNode->bresenhamParent = m_vNodes[s++];
			// sNode = m_vNodes[s];
			sNode->bresenhamParent = m_vNodes[d - 1];
			sNode = m_vNodes[d - 1];
			s = d - 1;
			d = s + 2; // d�� s�� ���� ���� ����
		}
		else
		{
			sNode->bresenhamParent = dNode;
			d++; // d�� ���� ����
		}

		InvalidateRect(m_hWnd, nullptr, false);
		UpdateWindow(m_hWnd);
	}
}

bool Bresenham::CalBresenham(int startY, int startX, int destY, int destX)
{
	bool ret = true;

	// ������ �Ǵ�
	int H = destY - startY;
	int W = destX - startX;

	// ������ ���� ���ϱ�
	float a = (float)H / W;

	if (0.0f < a && a <= 1.0f)
	{
		int sY = startY;
		int sX = startX;

		if (startX > destX)
		{
			sX = destX;
			sY = destY;
			destX = startX;
		}

		// �ʱⰪ
		int F = 2 * H - W;

		int dF1 = 2 * H;
		int dF2 = 2 * (H - W);

		for (sX; sX <= destX; sX++)
		{
			// �浹 �Ǵ�
			if (g_Tile[sY][sX] == TILE_TYPE::OBSTACLE)
				return false;

			if (F < 0)
				F += dF1;
			else
			{
				sY++;
				F += dF2;
			}
		}
	}
	else if (a > 1.0f)
	{
		int sY = startY;
		int sX = startX;

		if (startY > destY)
		{
			sX = destX;
			sY = destY;
			destY = startY;
		}

		// �ʱⰪ
		int F = 2 * W - H;

		int dF1 = 2 * W;
		int dF2 = 2 * (W - H);

		for (sY; sY <= destY; sY++)
		{
			// �浹 �Ǵ�
			if (g_Tile[sY][sX] == TILE_TYPE::OBSTACLE)
				return false;

			if (F < 0)
				F += dF1;
			else
			{
				sX++;
				F += dF2;
			}
		}
	}
	else // ���Ⱑ ������ ���
	{
		if (a >= -1.0f)
		{
			int sY = startY;
			int sX = startX;

			if (startX > destX)
			{
				sX = destX;
				sY = destY;
				destX = startX;
			}

			// �ʱⰪ
			int F = 2 * H + W;

			int dF1 = 2 * H;
			int dF2 = 2 * (H + W);

			for (sX; sX <= destX; sX++)
			{
				// �浹 �Ǵ�
				if (g_Tile[sY][sX] == TILE_TYPE::OBSTACLE)
					return false;

				if (F < 0)
				{
					sY--;
					F += dF2;
				}
				else
					F += dF1;
			}
		}
		else // a < -1.0f �� ���
		{
			int sY = startY;
			int sX = startX;

			if (startY > destY)
			{
				sX = destX;
				sY = destY;
				destY = startY;
			}

			// �ʱⰪ
			int F = 2 * W + H;

			int dF1 = 2 * W;
			int dF2 = 2 * (W + H);

			for (sY; sY <= destY; sY++)
			{
				// �浹 �Ǵ�
				if (g_Tile[sY][sX] == TILE_TYPE::OBSTACLE)
					return false;

				if (F < 0)
				{
					sX--;
					F += dF2;
				}
				else
					F += dF1;
			}
		}
	}

	return ret;
}