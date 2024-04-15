// RedBlackTree.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_REDBLACKTREE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_REDBLACKTREE);
	wcex.lpszClassName = L"ABCD";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(L"ABCD", L"Red Black Tree", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	FILE *conin = nullptr;
	FILE *conout = nullptr;
	FILE *conerr = nullptr;

	if (AllocConsole())
	{
		freopen_s(&conin, "CONIN$", "rb", stdin);
		freopen_s(&conout, "CONOUT$", "wb", stdout);
		freopen_s(&conerr, "CONOUT$", "wb", stderr);

		// std::ios::sync_with_stdio();
	}

	std::cout << "Red Black Tree program start" << std::endl;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

BinarySearchTree<int> g_Bst;
RBTree<int> g_rbtree;

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
    break;

    case WM_KEYDOWN:
    {
		int num;

        switch (wParam)
        {
		// 삽입
		case L'1':
			std::cout << "삽입 >> ";
			std::cin >> num;

			g_rbtree.Insert(num);
			break;
		// 삭제
		case L'2':
			// std::cout << "삭제 >> ";
			// std::cin >> num;
			// 
			// g_Bst.Delete(num);
			break;
		// 조회
		case L'3':
			g_rbtree.Print();
			break;
		// 전체 삭제
		case L'4':
			// g_Bst.Clear();
			break;

        }

        InvalidateRect(hWnd, NULL, false);
    }
    break;

    case WM_PAINT:
        {
		PatBlt(g_hMemDC, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, WHITENESS);
		
        // 여기에 g_hMemDC를 인자로 받는 그리기 함수 호출


		PAINTSTRUCT ps;
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, g_MemDCRect.right, g_MemDCRect.bottom, g_hMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		SelectObject(g_hMemDC, g_hMemDCBitmap_old);
		DeleteObject(g_hMemDC);
		DeleteObject(g_hMemDCBitmap);
		PostQuitMessage(0); // 종료시키는 명령
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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
