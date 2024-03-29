#include "pch.h"
#include "Setting.h"
#include "DrawGRID.h"

TILE_TYPE g_Tile[GRID_HEIGHT][GRID_WIDTH]; // 0 : 장애물 없음 / 1 : 장애물 있음

bool g_bErase = false;
bool g_bDrag = false;
int scrollUp = 0;

int renderStartY = 0;
int renderStartX = 0;

void RenderGrid(HDC hdc)
{
	int iX = 0;
	int iY = 0;

	SelectGDI pen = SelectGDI(hdc, PEN_TYPE::BLACK);

	// 그리드의 마지막 선을 추가로 그리기  위해 <= 의 반복 조건
	for (int iCntW = renderStartX; iCntW <= GRID_WIDTH; iCntW++)
	{
		MoveToEx(hdc, iX, 0, NULL);
		LineTo(hdc, iX, (GRID_HEIGHT - renderStartY) * (GRID_SIZE +scrollUp));
		iX += (GRID_SIZE + scrollUp);
	}

	for (int iCntH = renderStartY; iCntH <= GRID_HEIGHT; iCntH++)
	{
		MoveToEx(hdc, 0, iY, NULL);
		LineTo(hdc, (GRID_WIDTH - renderStartX) * (GRID_SIZE + scrollUp), iY);
		iY += (GRID_SIZE + scrollUp);
	}
}

void RenderObstacle(HDC hdc)
{
	int iX = 0;
	int iY = 0;

	
	SelectGDI pen = SelectGDI(hdc, PEN_TYPE::NULLPEN);

	for (int iCntW = 0; iCntW < GRID_WIDTH; iCntW++)
	{
		for (int iCntH = 0; iCntH < GRID_HEIGHT; iCntH++)
		{
			if ((UINT)g_Tile[iCntH][iCntW] == 0)
				continue;

			iX = iCntW * (GRID_SIZE + scrollUp) - renderStartX * (GRID_SIZE + scrollUp);
			iY = iCntH * (GRID_SIZE + scrollUp) - renderStartY * (GRID_SIZE + scrollUp);

			if ((UINT)g_Tile[iCntH][iCntW] == 1)
			{
				SelectGDI brush = SelectGDI(hdc, BRUSH_TYPE::GRAY);
				Rectangle(hdc, iX, iY, iX + (GRID_SIZE + scrollUp) + 2, iY + (GRID_SIZE + scrollUp) + 2);
				continue;
			}

			UINT brushType = (UINT)g_Tile[iCntH][iCntW];

			if (!(brushType == 0 || brushType == 1))
			{
				brushType -= (UINT)TILE_TYPE::GRAY;
			}

			SelectGDI brush = SelectGDI(hdc, (BRUSH_TYPE)brushType);
			Rectangle(hdc, iX, iY, iX + (GRID_SIZE + scrollUp) + 2, iY + (GRID_SIZE + scrollUp) + 2);
		}
	}
}

void RenderUI(HDC hdc)
{
	// SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 10, 10, L"출발지 설정 : 1, 도착지 설정 : 2", 22);
	TextOut(hdc, 10, 30, L"장애물 설치 : 클릭 후 드래그", 17);
	TextOut(hdc, 10, 50, L"마우스 휠 : 확대, 이동 : WASD", 21);
}
