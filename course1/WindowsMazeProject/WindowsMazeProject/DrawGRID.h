#pragma once

extern TILE_TYPE g_Tile[GRID_HEIGHT][GRID_WIDTH]; // 0 : 장애물 없음 / 1 : 장애물 있음

/*
타일의 속성 입력 / 제거 플래그
*/

extern bool g_bErase;
extern bool g_bDrag;

extern int scrollUp;

extern int renderStartY;
extern int renderStartX;

void RenderGrid(HDC hdc);

void RenderObstacle(HDC hdc);

void RenderUI(HDC hdc);