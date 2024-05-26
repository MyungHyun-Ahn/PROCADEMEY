#include "pch.h"
#include "Define.h"
#include "ConsoleManager.h"

ConsoleManager *g_ConsoleMgr = ConsoleManager::GetInstance();

ConsoleManager::ConsoleManager()
{
	Init();
}

ConsoleManager::~ConsoleManager()
{
}

void ConsoleManager::Init()
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	//-------------------------------------------------------------
	// ȭ���� Ŀ���� �Ⱥ��̰Բ� �����Ѵ�.
	//-------------------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			// Ŀ�� ũ��.
	// �̻��ϰԵ� 0 �̸� ���´�. 1���ϸ� �ȳ��´�.

	m_hScreen = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(m_hScreen, &stConsoleCursor);
}

void ConsoleManager::MoveCursor(int iPosY, int iPosX)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	//-------------------------------------------------------------
	// ���ϴ� ��ġ�� Ŀ���� �̵���Ų��.
	//-------------------------------------------------------------
	SetConsoleCursorPosition(m_hScreen, stCoord);
}

void ConsoleManager::BufferFlip()
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		MoveCursor(iCnt, 0);
		printf(m_szScreenBuffer[iCnt]);
	}
}

void ConsoleManager::BufferClear()
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memset(m_szScreenBuffer[iCnt], ' ', dfSCREEN_WIDTH);
		m_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

void ConsoleManager::SpriteDraw(const stPos &pos, char chSprite)
{
	if (pos.m_iX < 0 || pos.m_iY < 0 || pos.m_iX >= dfSCREEN_WIDTH - 1 || pos.m_iY >= dfSCREEN_HEIGHT)
		return;

	m_szScreenBuffer[pos.m_iY][pos.m_iX] = chSprite;
}

void ConsoleManager::ConsoleWrite(char buffer[][dfSCREEN_WIDTH])
{
	// �ܼ� ���ۿ� ����
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memcpy_s(m_szScreenBuffer[iCnt], dfSCREEN_WIDTH, buffer[iCnt], dfSCREEN_WIDTH);
		m_szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = (wchar_t)NULL;
	}
}

void ConsoleManager::WriteText(const stPos &pos, const char *text)
{
	memcpy_s(m_szScreenBuffer[pos.m_iY], dfSCREEN_WIDTH, text, strlen(text));
}