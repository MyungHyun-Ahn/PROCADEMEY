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

	g_Logger->WriteLogConsole(LOG_LEVEL::DEBUG, L"FPS : %d, Loop : %d, SyncCount : %d, AcceptTPS : %d, RecvTPS : %d, SendTPS : %d", g_FPS, g_LoopCount, g_SyncCount, g_AcceptTPS, g_RecvTPS, g_SendTPS);

	if (g_FPS != FRAME_PER_SECOND)
	{
		g_Logger->WriteLog(L"FPS_CHECK", LOG_LEVEL::SYSTEM, L"FPS : %d, Loop : %d, SyncCount : %d, AcceptTPS : %d, RecvTPS : %d, SendTPS : %d\n", g_FPS, g_LoopCount, g_SyncCount, g_AcceptTPS, g_RecvTPS, g_SendTPS);
	}

	g_mTime += 1000;
	g_LoopCount = 0;
	g_FPS = 0;
	g_AcceptTPS = 0;
	g_RecvTPS = 0;
	g_SendTPS = 0;
}

int main()
{
	srand((unsigned int)time(nullptr));

	g_Logger = Logger::GetInstance();
	g_Logger->SetDirectory(L"LogFile");
	g_Logger->SetLogLevel(LOG_LEVEL::DEBUG);

	g_NetworkMgr = NetworkManager::GetInstance();
	g_NetworkMgr->Start();

	timeBeginPeriod(1);

	int prevTick = timeGetTime();
	g_mTime = prevTick;
	while (true)
	{
		// ��Ʈ��ũ
		// Read
		// �ٷιٷ� �������� ���⼭ ������ �ȴ�.
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