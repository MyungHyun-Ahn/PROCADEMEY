#include "pch.h"
#include "Console.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"

HANDLE g_hScreen;
wchar_t g_szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

//-------------------------------------------------------------
// �ܼ� ��� ���� �غ� �۾�.
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
	// ȭ���� Ŀ���� �Ⱥ��̰Բ� �����Ѵ�.
	//-------------------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			// Ŀ�� ũ��.
	// �̻��ϰԵ� 0 �̸� ���´�. 1���ϸ� �ȳ��´�.

//-------------------------------------------------------------
// �ܼ�ȭ�� (���ٴٵ� �ƿ�ǲ) �ڵ��� ���Ѵ�.
//-------------------------------------------------------------
	g_hScreen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(g_hScreen, &stConsoleCursor);
}

//-------------------------------------------------------------
// �ܼ� ȭ���� Ŀ���� X, Y ��ǥ�� �̵���Ų��.
//
//-------------------------------------------------------------
void ConsoleMoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	//-------------------------------------------------------------
	// ���ϴ� ��ġ�� Ŀ���� �̵���Ų��.
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