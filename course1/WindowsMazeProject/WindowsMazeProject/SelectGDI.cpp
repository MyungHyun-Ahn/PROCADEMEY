#include "pch.h"

HBRUSH g_arrBrush[(UINT)BRUSH_TYPE::END];
HPEN g_arrPen[(UINT)PEN_TYPE::END];

SelectGDI::SelectGDI(HDC hdc, PEN_TYPE penType)
	: m_hDC(hdc)
	, m_hDefaultPen(nullptr)
	, m_hDefaultBrush(nullptr)
{
	HPEN hPen = g_arrPen[(UINT)penType];
	m_hDefaultPen = (HPEN)SelectObject(hdc, hPen);
}

SelectGDI::SelectGDI(HDC hdc, BRUSH_TYPE brushType)
	: m_hDC(hdc)
	, m_hDefaultPen(nullptr)
	, m_hDefaultBrush(nullptr)
{
	HBRUSH hBrush = g_arrBrush[(UINT)brushType];
	m_hDefaultBrush = (HBRUSH)SelectObject(hdc, hBrush);
}

SelectGDI::~SelectGDI()
{
	SelectObject(m_hDC, m_hDefaultPen);
	SelectObject(m_hDC, m_hDefaultBrush);
}

void InitGDIObject()
{
	// color brush - 流立 昏力 O
	g_arrBrush[(UINT)BRUSH_TYPE::GRAY] = (HBRUSH)CreateSolidBrush(RGB(100, 100, 100));
	g_arrBrush[(UINT)BRUSH_TYPE::RED] = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
	g_arrBrush[(UINT)BRUSH_TYPE::GREEN] = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
	g_arrBrush[(UINT)BRUSH_TYPE::BLUE] = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
	g_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	g_arrBrush[(UINT)BRUSH_TYPE::YELLOW] = (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
	g_arrBrush[(UINT)BRUSH_TYPE::PINK] = (HBRUSH)CreateSolidBrush(RGB(255, 192, 203));
	g_arrBrush[(UINT)BRUSH_TYPE::MAGENTA] = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
	g_arrBrush[(UINT)BRUSH_TYPE::TEAL] = (HBRUSH)CreateSolidBrush(RGB(0, 128, 128));

	// hollow brush - 流立 昏力 X
	g_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// color pen - 流立 昏力 O
	g_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	g_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	g_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	g_arrPen[(UINT)PEN_TYPE::BLACK] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	// null pen - 流立 昏力 X
	g_arrPen[(UINT)PEN_TYPE::NULLPEN] = (HPEN)GetStockObject(NULL_PEN);
}

void ReleaseGDIObject()
{
	for (int i = 0; i < (UINT)BRUSH_TYPE::STOCK_OBJECT_START; ++i)
	{
		DeleteObject(g_arrBrush[i]);
	}

	for (int i = 0; i < (UINT)PEN_TYPE::STOCK_OBJECT_START; ++i)
	{
		DeleteObject(g_arrPen[i]);
	}
}
