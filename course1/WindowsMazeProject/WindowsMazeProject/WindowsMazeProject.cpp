// WindowsProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "DrawGRID.h"
#include "resource.h"

#pragma warning(disable : 6385)
#pragma warning(disable : 6386)

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HDC hdc;

// 더블 버퍼링 관련 전역 변수
HBITMAP g_hMemDCBitmap;
HBITMAP g_hMemDCBitmap_old;
HDC g_hMemDC;
RECT g_MemDCRect;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSMAZEPROJECT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSMAZEPROJECT);
	wcex.lpszClassName = L"ABCD";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	// 애플리케이션 초기화를 수행합니다:
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// Define의 단점 - 오류났을 때 밑줄을 걸어주지 못한다.

	HWND hWnd = CreateWindowW(L"ABCD", L"A* Maze Route Search", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	InitGDIObject();

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ReleaseGDIObject();

	return (int)msg.wParam;
}

int xPos;
int yPos;
bool startPosOn = false;
bool exitPosOn = false;

bool modeG = false;
bool modeH = true;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_CREATE:
	{
		// 윈도우 생성시 현 윈도우 크기와 동일한 메모리 DC 생성
		GetClientRect(hWnd, &g_MemDCRect);
		g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
		g_hMemDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);
		g_hMemDCBitmap_old = (HBITMAP)SelectObject(g_hMemDC, g_hMemDCBitmap);
	}
	break;

	case WM_SIZE:
	{
		SelectObject(g_hMemDC, g_hMemDCBitmap_old);
		DeleteObject(g_hMemDC);
		DeleteObject(g_hMemDCBitmap);

		HDC hdc = GetDC(hWnd);

		GetClientRect(hWnd, &g_MemDCRect);
		g_hMemDCBitmap = CreateCompatibleBitmap(hdc, g_MemDCRect.right, g_MemDCRect.bottom);
		g_hMemDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);
		g_hMemDCBitmap_old = (HBITMAP)SelectObject(g_hMemDC, g_hMemDCBitmap);
	}

	case WM_LBUTTONDOWN:
		g_bDrag = true;
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			int iTileX = xPos / (GRID_SIZE + scrollUp) + renderStartX;
			if (iTileX > GRID_WIDTH)
			{
				break;
			}

			int iTileY = yPos / (GRID_SIZE + scrollUp) + renderStartY;
			if (iTileY > GRID_WIDTH)
			{
				break;
			}

			if ((UINT)g_Tile[iTileY][iTileX] == 1)
				g_bErase = true;
			else
				g_bErase = false;
		}
		break;

	case WM_LBUTTONUP:
		g_bDrag = false;
		break;

	case WM_MOUSEMOVE:
	{
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		if (g_bDrag)
		{

			int iTileX = xPos / (GRID_SIZE + scrollUp) + renderStartX;
			int iTileY = yPos / (GRID_SIZE + scrollUp) + renderStartY;

			if (iTileX == startX && iTileY == startY)
				break;

			if (iTileX == destX && iTileY == destY)
				break;


			g_Tile[iTileY][iTileX] = (TILE_TYPE)!g_bErase;
			InvalidateRect(hWnd, NULL, false);
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		// 땡긴 위치의 x, y 좌표 구하기
		int iTileX = xPos / (GRID_SIZE + scrollUp) + renderStartX;
		int iTileY = yPos / (GRID_SIZE + scrollUp) + renderStartY;

		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (zDelta < 0)
		{
			if (scrollUp > -8)
				scrollUp -= 8;
		}

		if (zDelta > 0)
		{
			if (scrollUp <= 64)
				scrollUp += 8;
		}

		renderStartX = iTileX - xPos / (GRID_SIZE + scrollUp);
		if (renderStartX < 0)
			renderStartX = 0;

		renderStartY = iTileY - yPos / (GRID_SIZE + scrollUp);
		if (renderStartY < 0)
			renderStartY = 0;

		InvalidateRect(hWnd, NULL, false);
	}
	break;

	case WM_KEYDOWN:
	{
		int iTileX = xPos / (GRID_SIZE + scrollUp) + renderStartX;
		if (iTileX >= GRID_WIDTH)
		{
			break;
		}

		int iTileY = yPos / (GRID_SIZE + scrollUp) + renderStartY;
		if (iTileY >= GRID_HEIGHT)
		{
			break;
		}

		switch (wParam)
		{
		case L'1':
		{
			if (startPosOn && iTileY == startY && iTileX == startX)
			{
				startPosOn = false;
				g_Tile[iTileY][iTileX] = TILE_TYPE::NONE_OBSTACLE;
			}
			else if (!startPosOn)
			{
				startPosOn = true;
				startX = iTileX;
				startY = iTileY;
				g_Tile[iTileY][iTileX] = TILE_TYPE::GREEN;
			}		
		}
		break;

		case L'2':
		{
			if (exitPosOn && iTileY == destY && iTileX == destX)
			{
				exitPosOn = false;
				g_Tile[iTileY][iTileX] = TILE_TYPE::NONE_OBSTACLE;
			}
			else if (!exitPosOn)
			{
				exitPosOn = true;
				destX = iTileX;
				destY = iTileY;
				g_Tile[iTileY][iTileX] = TILE_TYPE::RED;
			}
		}
		break;

		case L'3':
		{

		}
		break;
		
		case L'4':
		{

		}
		break;

		case L'5':
		{
			AStar astar(modeG, modeH);
			astar.Search(hWnd, g_hMemDC);
			InvalidateRect(hWnd, NULL, false);
		}
		break;

		case L'W':
		{
			renderStartY -= 1;
			if (renderStartY < 0)
			{
				renderStartY = 0;
			}
		}
		break;

		case L'A':
		{
			renderStartX -= 1;
			if (renderStartX < 0)
			{
				renderStartX = 0;
			}
		}
		break;

		case L'S':
		{
			renderStartY += 1;
			if (renderStartY >= GRID_HEIGHT)
			{
				renderStartY = GRID_HEIGHT - 1;
			}
		}
		break;

		case L'D':
		{
			renderStartX += 1;
			if (renderStartX >= GRID_WIDTH)
			{
				renderStartX = GRID_WIDTH - 1;
			}
		}
		break;
		}

		InvalidateRect(hWnd, NULL, false);
	}
	break;

	case WM_PAINT:
	{
		PatBlt(g_hMemDC, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, WHITENESS);

		RenderObstacle(g_hMemDC);
		RenderGrid(g_hMemDC);
		RenderUI(g_hMemDC);
		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY: // 윈도우 창이 꺼지면서 발생하는 메시지
		SelectObject(g_hMemDC, g_hMemDCBitmap_old);
		DeleteObject(g_hMemDC);
		DeleteObject(g_hMemDCBitmap);
		PostQuitMessage(0); // 종료시키는 명령
		break;
	default: // x 버튼을 눌렀을 때 여기로 넘어오고 이후 WM_DESTROY 메시지 발생
		return DefWindowProc(hWnd, message, wParam, lParam); // 처리하지 않은 메시지는 여기로 들어간다.
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
