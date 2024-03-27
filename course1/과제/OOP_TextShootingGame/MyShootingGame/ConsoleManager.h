#pragma once
class ConsoleManager
{
	SINGLE(ConsoleManager)

public:
	void				Init();
	void				MoveCursor(int iPosY, int iPosX);
	void				BufferFlip();
	void				BufferClear();
	void				SpriteDraw(const stPos &pos, char chSprite);
	void				ConsoleWrite(char buffer[][dfSCREEN_WIDTH]);

private:
	HANDLE m_hScreen;
	char m_szScreenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
};

extern ConsoleManager *g_ConsoleMgr;