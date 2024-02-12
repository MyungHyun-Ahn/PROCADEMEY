#include "pch.h"
#include "Console.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"

HANDLE g_hScreen;
wchar_t g_szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

//-------------------------------------------------------------
// 콘솔 제어를 위한 준비 작업.
//
//-------------------------------------------------------------
void ConsoleInitial(void)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1280, 720, TRUE);


	CONSOLE_CURSOR_INFO stConsoleCursor;

	//-------------------------------------------------------------
	// 화면의 커서를 안보이게끔 설정한다.
	//-------------------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			// 커서 크기.
	// 이상하게도 0 이면 나온다. 1로하면 안나온다.

//-------------------------------------------------------------
// 콘솔화면 (스텐다드 아웃풋) 핸들을 구한다.
//-------------------------------------------------------------
	g_hScreen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(g_hScreen, &stConsoleCursor);
}

//-------------------------------------------------------------
// 콘솔 화면의 커서를 X, Y 좌표로 이동시킨다.
//
//-------------------------------------------------------------
void ConsoleMoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	//-------------------------------------------------------------
	// 원하는 위치로 커서를 이동시킨다.
	//-------------------------------------------------------------
	SetConsoleCursorPosition(g_hScreen, stCoord);
}

void ConsoleBufferFlip(void)
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		ConsoleMoveCursor(0, iCnt);
		wprintf(g_szScreenBuffer[iCnt]);
	}
}

void ConsoleBufferClear(void)
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		wmemset(g_szScreenBuffer[iCnt], L' ', dfSCREEN_WIDTH);
		g_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
}

void ConsoleSpriteDraw(const stPos &pos, wchar_t chSprite)
{
	if (pos.m_iX < 0 || pos.m_iY < 0 || pos.m_iX >= dfSCREEN_WIDTH - 1 || pos.m_iY >= dfSCREEN_HEIGHT)
		return;

	g_szScreenBuffer[pos.m_iY][pos.m_iX] = chSprite;
}

void ConsoleRender(void)
{
	ConsoleBufferClear();
	PlayerDraw();
	EnemyDraw();
	MissileDraw();
	ConsoleBufferFlip();
}