#include "pch.h"
#include "Define.h"
#include "NetworkManager.h"

int main()
{
	g_NetworkMgr = NetworkManager::GetInstance();
	g_NetworkMgr->Start();

	timeBeginPeriod(1);
	int prevTick = timeGetTime();
	int seconds = prevTick;
	int fpsCount = 0;
	while (true)
	{
		// ��Ʈ��ũ


		if (int time = timeGetTime(); time - prevTick >= dfTICK_PER_FRAME)
		{
			// ����

			prevTick += dfTICK_PER_FRAME;
		}
	}


	timeEndPeriod(1);

	return 0;
}