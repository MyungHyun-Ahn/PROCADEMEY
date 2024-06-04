// NetDraw_Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include "framework.h"
#include "NetDraw_Client.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windowsx.h>
#include <iostream>
#include "RingBuffer.h"

#define UM_NETWORK (WM_USER + 1)

#define IP L"127.0.0.1"
#define PORT 25000

#define MAX_LOADSTRING 100

// 헤더
#pragma pack(push, 1)
struct st_DRAW_PACKET
{
    unsigned short Len;
    int		iStartX;
    int		iStartY;
    int		iEndX;
    int		iEndY;
};
#pragma pack(pop)

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HWND g_hWnd;

SOCKET serverSock;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

RingBuffer g_sendBuffer;
RingBuffer g_recvBuffer;

void WriteEvent();
void ReadEvent();
void ProcessPacket(HDC hdc);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    int retVal;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NETDRAWCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET)
    {
        return 0;
    }

    retVal = WSAAsyncSelect(serverSock, g_hWnd, UM_NETWORK, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    if (retVal == SOCKET_ERROR)
    {
        return 0;
    }

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPtonW(AF_INET, IP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(PORT);

    retVal = connect(serverSock, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (retVal == SOCKET_ERROR)
    {
        int errVal = WSAGetLastError();
        if (errVal == WSAEWOULDBLOCK)
        {
            // 에러 아님
        }
        else
            return 1;
    }

	HWND hWnd = CreateWindowW(L"ABCD", L"Debug Console", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// FILE *conin = nullptr;
	// FILE *conout = nullptr;
	// FILE *conerr = nullptr;
    // 
	// if (AllocConsole())
	// {
	// 	freopen_s(&conin, "CONIN$", "rb", stdin);
	// 	freopen_s(&conout, "CONOUT$", "wb", stdout);
	// 	freopen_s(&conerr, "CONOUT$", "wb", stderr);
    // 
	// 	std::ios::sync_with_stdio();
	// }
    // 
    

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
	// fclose(conin);
	// fclose(conout);
	// fclose(conerr);

    WSACleanup();

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETDRAWCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NETDRAWCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
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

bool isConnected = false;
bool isDraged = false;

int oldX = -1004;
int oldY = -1004;
int curX = -1004;
int curY = -1004;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            ProcessPacket(hdc);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_LBUTTONDOWN:
    {
        isDraged = true;
		curX = GET_X_LPARAM(lParam);
		curY = GET_Y_LPARAM(lParam);
    }
        break;

	case WM_LBUTTONUP:
    {
        isDraged = false;
    }
		break;

    case WM_MOUSEMOVE:
    {
        // 연결 이후부터
        if (!isConnected)
            break;

        if (!isDraged)
            break;

		oldX = curX;
		oldY = curY;

        curX = GET_X_LPARAM(lParam);
        curY = GET_Y_LPARAM(lParam);

		st_DRAW_PACKET drawPacket;
		drawPacket.Len = sizeof(drawPacket) - 2;
		drawPacket.iStartX = oldX;
		drawPacket.iStartY = oldY;
		drawPacket.iEndX = curX;
		drawPacket.iEndY = curY;

		g_sendBuffer.Enqueue((char *)&drawPacket, sizeof(st_DRAW_PACKET));

		WriteEvent();
    }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case UM_NETWORK:
    {
        int errVal = WSAGETSELECTERROR(lParam);
        if (errVal)
        {
            // std::cout << "UM_NETWORK : " << errVal << std::endl;
            return errVal;
        }

        SOCKET serverSocket = (SOCKET)wParam;

        // 메시지 처리
        switch (WSAGETSELECTEVENT(lParam))
        {
        case FD_CONNECT:
        {
			// CONNECT가 온 이후부터
			isConnected = true;
			// std::cout << "Connect Success" << std::endl;
			// std::cout << sizeof(st_DRAW_PACKET) << std::endl;
        }
            break;

        case FD_CLOSE:
        {
			closesocket(serverSocket);

        }
            break;

        case FD_READ:
        {
			ReadEvent();
			InvalidateRect(hWnd, nullptr, FALSE);
        }
            break;

        case FD_WRITE:
        {
			WriteEvent();
        }
            break;
        }

    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 보내기
void WriteEvent()
{
    int directSize = g_sendBuffer.DirectDequeueSize();
    // std::cout << "Write : " << directSize << std::endl;

    int retVal = send(serverSock, g_sendBuffer.GetFrontPtr(), directSize, 0);
    if (retVal == SOCKET_ERROR)
    {
        int errVal = WSAGetLastError();
        if (errVal != WSAEWOULDBLOCK)
        {
            // WOULDBLOCK
            closesocket(serverSock);
            // std::cout << "WriteEvent() : " << errVal << std::endl;
            return;
        }
    }

    if (directSize == retVal)
    {
        g_sendBuffer.Clear();
        return;
    }

    g_sendBuffer.MoveFront(retVal);
}

// 읽기
void ReadEvent()
{
    int directSize = g_recvBuffer.DirectEnqueueSize();
    int retVal = recv(serverSock, g_recvBuffer.GetRearPtr(), directSize, 0);
    if (retVal == SOCKET_ERROR)
    {
		int errVal = WSAGetLastError();
		if (errVal != WSAEWOULDBLOCK)
		{
            closesocket(serverSock);
            // std::cout << "ReadEvent() : " << errVal << std::endl;
            return;
		}
    }

    g_recvBuffer.MoveRear(retVal);
}

void ProcessPacket(HDC hdc)
{
    int size = g_recvBuffer.GetUseSize();
    std::cout << "Size : " << size << std::endl;

    while (size >= sizeof(st_DRAW_PACKET))
    {
        st_DRAW_PACKET *packet = (st_DRAW_PACKET *)g_recvBuffer.GetFrontPtr();

        // g_recvBuffer.Dequeue((char *)&packet, sizeof(st_DRAW_PACKET));
		// std::cout << "StartX : " << packet->iStartX << " StartY : " << packet->iStartY << std::endl;
		// std::cout << "EndX : " << packet->iEndX << " EndY : " << packet->iEndY << std::endl;

        MoveToEx(hdc, packet->iStartX, packet->iStartY, NULL);
        LineTo(hdc, packet->iEndX, packet->iEndY);

        g_recvBuffer.MoveFront(sizeof(st_DRAW_PACKET));

        size = g_recvBuffer.GetUseSize();
    }
}