#pragma once

// 0 Ȥ�� 1�� �ƴ� ��
// 3�� ���ָ� BRUSH_TYPE�� �÷���
enum class TILE_TYPE
{
	NONE_OBSTACLE = 0,
	OBSTACLE,
	GRAY,
	RED,
	GREEN,
	BLUE,
	BLACK,
	YELLOW,
	PINK,
};

enum class BRUSH_TYPE
{
	GRAY,
	RED,
	GREEN,
	BLUE,
	BLACK,
	YELLOW,
	PINK,
	STOCK_OBJECT_START,
	HOLLOW,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	BLACK,
	STOCK_OBJECT_START,
	NULLPEN,
	END,
};
