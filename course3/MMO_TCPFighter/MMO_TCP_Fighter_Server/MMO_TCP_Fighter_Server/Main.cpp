#include "pch.h"
#include "Define.h"
#include "CPlayer.h"
#include "CSession.h"
#include "CNetworkManager.h"
#include "GameLogic.h"

DWORD g_mTime;
DWORD g_FPS = 0;
DWORD g_LoopCount = 0;

CCrashDump crashDump;

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

// 모든 싱글톤 객체의 초기화를 이곳에서
void InitializeSingleton()
{
	g_Logger = CLogger::GetInstance();
	g_Logger->SetDirectory(L"LogFile");
	g_Logger->SetLogLevel(LOG_LEVEL::DEBUG);

	g_Profiler = CProfileManager::GetInstance();
	g_NetworkMgr = CNetworkManager::GetInstance();
}

int main()
{
	srand((unsigned int)time(nullptr));

	InitializeSingleton();

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
			prevTick += TICK_PER_FRAME;
			g_FPS++;
		}

		Monitor();

		g_NetworkMgr->DisconnectClients();
	}

	timeEndPeriod(1);
	return 0;
}