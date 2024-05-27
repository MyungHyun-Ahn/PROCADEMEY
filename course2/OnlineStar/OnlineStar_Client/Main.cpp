#include "pch.h"
#include "Define.h"
#include "ConsoleManager.h"
#include "KeyManager.h"
#include "Packet.h"
#include "NetworkManager.h"
#include "Star.h"

int myID = -1234;
std::map<int, Star> stars;
int processPackets = 0;

bool Move()
{
	Star &myStar = stars[myID];

	stPos pos = myStar.m_Pos;

	if (KEY_TAP(KEY::UP) || KEY_HOLD(KEY::UP))
	{
		pos.m_iY -= 1;
	}
	
	if (KEY_TAP(KEY::DOWN) || KEY_HOLD(KEY::DOWN))
	{
		pos.m_iY += 1;
	}
	
	if (KEY_TAP(KEY::LEFT) || KEY_HOLD(KEY::LEFT))
	{
		pos.m_iX -= 1;
	}
	
	if (KEY_TAP(KEY::RIGHT) || KEY_HOLD(KEY::RIGHT))
	{
		pos.m_iX += 1;
	}

	// 좌표 유효성 체크
	pos.m_iY = max(pos.m_iY, 0);
	pos.m_iY = min(pos.m_iY, dfSCREEN_HEIGHT - 1);
	pos.m_iX = max(pos.m_iX, 0);
	pos.m_iX = min(pos.m_iX, dfSCREEN_WIDTH - 2);

	if (pos.m_iX == myStar.m_Pos.m_iX && pos.m_iY == myStar.m_Pos.m_iY)
	{
		return false;
	}

	myStar.Move(pos);
	return true;
}

bool ProcessPacket()
{
	// 16보다 작으면 Packet 처리하지 않음
	if (g_BufferSize < 16)
		return false;

	// 0 부터 파싱 시작
	int curPos = 0;
	while (curPos < g_BufferSize)
	{
		PacketBase *packetBase = (PacketBase *)(g_Buffer + curPos);
		PacketCode code = (PacketCode)packetBase->Type;
			switch (code)
			{
			case PacketCode::AllocID:
			{
				PacketAllocID *packet = (PacketAllocID *)packetBase;
				myID = packet->ID;

				Star myStar;
				myStar.m_Pos = { 0, 0 };

				stars[myID] = myStar;
			}
				break;
			case PacketCode::CreateStar:
			{
				PacketCreateStar *packet = (PacketCreateStar *)packetBase;

				Star star;
				star.m_Pos = { packet->X, packet->Y };
				stars[packet->ID] = star;
			}
				break;
			case PacketCode::DestroyStar:
			{
				PacketDestroyStar *packet = (PacketDestroyStar *)packetBase;
				stars.erase(packet->ID);
			}
				break;
			case PacketCode::MoveStar:
			{
				PacketMoveStar *packet = (PacketMoveStar *)packetBase;
				stars[packet->ID].m_Pos.m_iX = packet->X;
				stars[packet->ID].m_Pos.m_iY = packet->Y;
			}
				break;
			}
		curPos += 16;
		processPackets++;
	}

	g_BufferSize = 0;
	return true;
}

unsigned __stdcall RenderThreadFunc(void *pArg)
{
	while (true)
	{
		// 렌더
		g_ConsoleMgr->BufferClear();

		for (auto &star : stars)
		{
			g_ConsoleMgr->SpriteDraw(star.second.m_Pos, '*');
		}

		stPos textPos = { 0, 0 };
		CHAR text[50];
		sprintf_s(text, "Connect Client:%d Packet:%d", (int)stars.size(), processPackets);
		processPackets = 0;

		g_ConsoleMgr->WriteText(textPos, text);

		g_ConsoleMgr->BufferFlip();
	}
	
	return 0;
}

int main()
{
	std::locale::global(std::locale("Korean"));

	// Manager Class Initialize
	g_ConsoleMgr = ConsoleManager::GetInstance();
	g_KeyMgr = KeyManager::GetInstance();
	g_NetworkMgr = NetworkManager::GetInstance();

	// input IP
	CHAR ip[16];
	printf("접속할 IP 주소를 입력하세요 : ");
	scanf_s("%s", ip, 16);

	g_NetworkMgr->RegisterIP(ip);
	if (!g_NetworkMgr->Connect())
		return 1;

	g_NetworkMgr->SetNonBlockingOpt();
	if (!g_NetworkMgr->SetNonBlockingOpt())
		return 1;

	// HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, RenderThreadFunc, NULL, 0, NULL);

	while (true)
	{
		// Input
		g_KeyMgr->Update();
		
		if (myID != -1234)
		{
			// false : 패킷을 보낼 필요가 없음
			if (Move())
			{
				PacketMoveStar packet;
				packet.ID = myID;
				packet.Type = (int)PacketCode::MoveStar;
				packet.X = stars[myID].m_Pos.m_iX;
				packet.Y = stars[myID].m_Pos.m_iY;

				if (!g_NetworkMgr->Send((PacketBase *)&packet))
				{
					return 1;
				}

			}
		}
		
		// 네트워크
		if (!g_NetworkMgr->Select())
			return 1;

		// 패킷 수신한 것 처리
		ProcessPacket();

		// 렌더
		g_ConsoleMgr->BufferClear();
		
		for (auto &star : stars)
		{
			g_ConsoleMgr->SpriteDraw(star.second.m_Pos, '*');
		}
		
		stPos textPos = { 0, 0 };
		CHAR text[50];
		sprintf_s(text, "Connect Client:%d Packet:%d", (int)stars.size(), processPackets);
		processPackets = 0;
		
		g_ConsoleMgr->WriteText(textPos, text);
		
		g_ConsoleMgr->BufferFlip();

		// Sleep(10);
	}

	// CloseHandle(hThread);

	return 0;
}