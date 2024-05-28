#include "pch.h"
#include "Define.h"
#include "Packet.h"
#include "Star.h"
#include "NetworkManager.h"
#include "ConsoleManager.h"

int main()
{
	g_NetworkMgr = NetworkManager::GetInstance();
	g_ConsoleMgr = ConsoleManager::GetInstance();


	g_NetworkMgr->Start();

	while (true)
	{
		g_NetworkMgr->Select();

		// ·»´õ
		g_ConsoleMgr->BufferClear();
		
		for (auto &star : g_Stars)
		{
			g_ConsoleMgr->SpriteDraw(star.second.m_Pos, '*');
		}
		
		stPos textPos = { 0, 0 };
		CHAR text[50];
		sprintf_s(text, "Connect Client:%d", (int)g_Stars.size());
		
		g_ConsoleMgr->WriteText(textPos, text);
		
		g_ConsoleMgr->BufferFlip();
	}
}