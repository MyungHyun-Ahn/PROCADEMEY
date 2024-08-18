#include "pch.h"
#include "Define.h"
#include "Player.h"
#include "Session.h"
#include "NetworkManager.h"
#include "GameLogic.h"

DWORD g_mTime;
DWORD g_FPS = 0;
DWORD g_LoopCount = 0;
void Monitor()
{
	DWORD dTime = timeGetTime() - g_mTime;
	if (dTime < 1000)
		return;

	wprintf(L"LC : %d, FPS : %d\n", g_LoopCount, g_FPS);
	g_mTime += 1000;
	g_LoopCount = 0;
	g_FPS = 0;
}

int main()
{
	srand((unsigned int)time(nullptr));

	g_NetworkMgr = NetworkManager::GetInstance();
	g_NetworkMgr->Start();

	timeBeginPeriod(1);

	int prevTick = timeGetTime();
	g_mTime = prevTick;
	while (true)
	{
		// 네트워크
		// Read
		// 바로바로 보낼것은 여기서 보내도 된다.
		g_LoopCount++;
		g_NetworkMgr->Select();

		if (int time = timeGetTime(); time - prevTick >= TICK_PER_FRAME)
		{
			GameLogic::Update();
			// g_NetworkMgr->WriteSelect();
			// g_NetworkMgr->TimeoutCheck();
			prevTick += TICK_PER_FRAME;
			g_FPS++;
		}

		Monitor();

		g_NetworkMgr->DisconnectClients();
	}

	timeEndPeriod(1);
	return 0;
}