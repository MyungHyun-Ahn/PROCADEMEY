#pragma once

extern TILE_TYPE g_Tile[GRID_HEIGHT][GRID_WIDTH]; // 0 : ��ֹ� ���� / 1 : ��ֹ� ����

/*
Ÿ���� �Ӽ� �Է� / ���� �÷���
*/

extern bool g_bErase;
extern bool g_bDrag;

extern int scrollUp;

extern int renderStartY;
extern int renderStartX;

void RenderGrid(HDC hdc);

void RenderObstacle(HDC hdc);

void RenderUI(HDC hdc);