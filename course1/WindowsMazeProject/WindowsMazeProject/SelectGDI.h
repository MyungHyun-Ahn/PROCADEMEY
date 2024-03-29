#pragma once
class SelectGDI
{
public:
	SelectGDI(HDC hdc, PEN_TYPE penType);
	SelectGDI(HDC hdc, BRUSH_TYPE brushType);
	~SelectGDI();

private:
	HDC		m_hDC;
	HPEN	m_hDefaultPen;
	HBRUSH	m_hDefaultBrush;
};

void InitGDIObject();
void ReleaseGDIObject();

// 자주 사용하는 GDI Object
extern HBRUSH g_arrBrush[(UINT)BRUSH_TYPE::END];
extern HPEN g_arrPen[(UINT)PEN_TYPE::END];
