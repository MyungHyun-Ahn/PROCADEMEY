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

const WCHAR *g_Option1 = L"멘하튼";
const WCHAR *g_Option2 = L"유클리드";

bool modeG = false;
bool modeH = true;

void RenderUI(HDC hdc)
{
	// SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 10, 10, L"출발지 설정 : 1, 도착지 설정 : 2, 경로 탐색 : 5", 33);
	TextOut(hdc, 10, 30, L"장애물 설치 : 클릭 후 드래그", 17);
	TextOut(hdc, 10, 50, L"확대 : 마우스 휠, 이동 : WASD, 리셋 : R", 29);

	WCHAR messge[100];

	const WCHAR *opG;
	const WCHAR *opH;

	if (modeG)
	{
		opG = g_Option1;
	}
	else
	{
		opG = g_Option2;
	}

	if (modeH)
	{
		opH = g_Option1;
	}
	else
	{
		opH = g_Option2;
	}

	wsprintf(messge, L"가중치 옵션 변경: 3(G, %s), 4(H, %s)", opG, opH);
	TextOut(hdc, 10, 70, messge, (int)wcslen(messge));
}

void RenderInfo(HDC hdc)
{
	int Grid = GRID_SIZE + scrollUp;
	int halfGrid = (GRID_SIZE + scrollUp) / 2;

	WCHAR buffer[50];

	for (int y = renderStartY; y < GRID_HEIGHT; y++)
	{
		for (int x = renderStartX; x < GRID_WIDTH; x++)
		{
			// 그리드 사이즈
			// (GRID_SIZE + scrollUp) * g_arrNodes[y][x]->y;

			// 왼쪽 위 시작 좌표
			// 기본일 때
			// (GRID_SIZE + scrollUp) * g_arrNodes[y][x]->x;
			// 그럼 renderStartX가 들어가면
			// (GRID_SIZE + scrollUp) * (g_arrNodes[y][x]->x - renderStartX)
			// (GRID_SIZE + scrollUp) * (g_arrNodes[y][x]->y - renderStartY)
			
			if (g_arrNodes[y][x] == NULL)
				continue;

			int myLeftX = Grid * (g_arrNodes[y][x]->x - renderStartX);
			int myUpY = Grid * (g_arrNodes[y][x]->y - renderStartY);
			int myDownY = Grid * (g_arrNodes[y][x]->y - renderStartY + 1);

			if (scrollUp > 16)
			{
				SetBkMode(hdc, TRANSPARENT);

				swprintf_s(buffer, L"%.03lf", g_arrNodes[y][x]->G);
				TextOut(hdc, myLeftX, myUpY, buffer, wcslen(buffer));

				swprintf_s(buffer, L"%.03lf", g_arrNodes[y][x]->H);
				TextOut(hdc, myLeftX, (myUpY + myDownY) / 2 - 7, buffer, wcslen(buffer));

				swprintf_s(buffer, L"%.03lf", g_arrNodes[y][x]->F);
				TextOut(hdc, myLeftX, myDownY - 15, buffer, wcslen(buffer));
			}

			if (g_arrNodes[y][x]->parent == NULL)
				continue;

			int parentLeftX = Grid * (g_arrNodes[y][x]->parent->x - renderStartX);
			int parentUpY = Grid * (g_arrNodes[y][x]->parent->y - renderStartY);

			// 부모의 중심
			MoveToEx(hdc, (parentLeftX + myLeftX + Grid) / 2, (parentUpY + myUpY + Grid) / 2, NULL);
			LineTo(hdc, myLeftX + halfGrid, myUpY + halfGrid);
		}
	}

	Node *nextNode = curNode;

	while (nextNode != nullptr)
	{

		int myLeftX = Grid * (nextNode->x - renderStartX);
		int myUpY = Grid * (nextNode->y - renderStartY);

		if (nextNode->parent == nullptr)
			break;

		int parentLeftX = Grid * (nextNode->parent->x - renderStartX);
		int parentUpY = Grid * (nextNode->parent->y - renderStartY);

		MoveToEx(hdc, parentLeftX + halfGrid , parentUpY + halfGrid, NULL);
		LineTo(hdc, myLeftX + halfGrid, myUpY + halfGrid);

		nextNode = nextNode->parent;
	}
}
