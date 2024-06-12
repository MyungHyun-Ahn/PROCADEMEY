#include "pch.h"
#include "Define.h"
#include "Player.h"
#include "Session.h"
#include "Packet.h"
#include "NetworkManager.h"
#include "GameLogic.h"

int main()
{
	srand(time(nullptr));

	g_NetworkMgr = NetworkManager::GetInstance();
	g_NetworkMgr->Start();

	timeBeginPeriod(1);

	int prevTick = timeGetTime();
	while (true)
	{
		// 네트워크
		// Read
		// 바로바로 보낼것은 여기서 보내도 된다.
		g_NetworkMgr->ReadSelect();
		g_NetworkMgr->DisconnectClients();


		if (int time = timeGetTime(); time - prevTick >= dfTICK_PER_FRAME)
		{
			g_NetworkMgr->WriteSelect();
			g_NetworkMgr->DisconnectClients();

			// 로직
			GameLogic::Update();

			prevTick += dfTICK_PER_FRAME;
		}
	}

	timeEndPeriod(1);
	return 0;
}