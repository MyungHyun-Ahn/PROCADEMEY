#include "pch.h"
#include "Define.h"
#include "DefinePacket.h"
#include "Session.h"
#include "Player.h"
#include "NetworkManager.h"
#include "GenPacket.h"
#include "ProcessPacket.h"

SerializableBuffer &operator>>(SerializableBuffer &sBuffer, PacketHeader &header)
{
	sBuffer.Dequeue((char *)&header, sizeof(header));
	return sBuffer;
}

// false 일 시 연결 끊어주기

bool ProcessPacket::PacketProc_MoveStart(Session *pSession, char *pPacket)
{
	PacketCSMoveStart *packet = (PacketCSMoveStart *)pPacket;

	wprintf(L"MoveStart # Player Id %d # x : %d, y : %d ", pSession->m_Id, packet->x, packet->y);
	wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id].m_X, g_Players[pSession->m_Id].m_Y);

	if (abs(g_Players[pSession->m_Id].m_X - packet->x) > ERROR_RANGE ||
		abs(g_Players[pSession->m_Id].m_Y - packet->y) > ERROR_RANGE)
	{
		return false;
	}

	g_Players[pSession->m_Id].m_Action = (DWORD)packet->moveDir;

	switch ((MOVE_DIR)packet->moveDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		g_Players[pSession->m_Id].m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		g_Players[pSession->m_Id].m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	g_Players[pSession->m_Id].m_X = packet->x;
	g_Players[pSession->m_Id].m_Y = packet->y;

	{
		PacketSCMoveStart newPacket;
		makePacketSCMoveStart(&newPacket, pSession->m_Id, packet->moveDir, packet->x, packet->y);
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)&newPacket, sizeof(PacketSCMoveStart));
	}

	return true;
}

bool ProcessPacket::PacketProc_MoveStop(Session *pSession, char *pPacket)
{
	PacketCSMoveStop *packet = (PacketCSMoveStop *)pPacket;

	wprintf(L"MoveStop # Player Id %d # x : %d, y : %d ", pSession->m_Id, packet->x, packet->y);
	wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id].m_X, g_Players[pSession->m_Id].m_Y);

	if (abs(g_Players[pSession->m_Id].m_X - packet->x) > ERROR_RANGE ||
		abs(g_Players[pSession->m_Id].m_Y - packet->y) > ERROR_RANGE)
	{
		return false;
	}

	g_Players[pSession->m_Id].m_Action = (DWORD)MOVE_DIR::MOVE_DIR_STOP;

	switch ((MOVE_DIR)packet->viewDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		g_Players[pSession->m_Id].m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		g_Players[pSession->m_Id].m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	g_Players[pSession->m_Id].m_X = packet->x;
	g_Players[pSession->m_Id].m_Y = packet->y;

	{
		PacketSCMoveStop newPacket;

		// 헤더 기입
		newPacket.byCode = PACKET_IDENTIFIER;
		newPacket.bySize = sizeof(PacketSCMoveStop) - sizeof(PacketHeader);
		newPacket.byType = (BYTE)PACKET_CODE::PACKET_SC_MOVE_STOP;

		// 페이로드
		newPacket.id = pSession->m_Id;
		newPacket.direction = g_Players[pSession->m_Id].m_Direction;
		newPacket.x = packet->x;
		newPacket.y = packet->y;

		// SendPacket Broadcast
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)&newPacket, sizeof(PacketSCMoveStop));
	}

	return true;
}

bool ProcessPacket::PacketProc_Attack1(Session *pSession, char *pPacket)
{
	PacketCSAttack1 *packet = (PacketCSAttack1 *)pPacket;

	wprintf(L"Attack1 # Player Id %d # x : %d, y : %d ", pSession->m_Id, packet->x, packet->y);
	wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id].m_X, g_Players[pSession->m_Id].m_Y);

	MOVE_DIR eDir = (MOVE_DIR)packet->direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	for (auto &p : g_Players)
	{
		Player &player = p.second;

		if (player.m_Id == pSession->m_Id)
			continue;

		int dX = (player.m_X - packet->x) * dir;
		int dY = abs(player.m_Y - packet->y);
		// 음수면 안 맞은 것
		if (dX < 0)
			continue;

		if (!(dX < ATTACK1_RANGE_X && dY < ATTACK1_RANGE_Y))
			continue;

		player.m_Hp -= ATTACK1_DAMAGE;

		if (player.m_Hp <= 0)
		{
			if (g_Sessions[player.m_Id]->m_isVaild)
			{
				g_NetworkMgr->deleteQueue.push_back(g_Sessions[player.m_Id]);
				g_Sessions[player.m_Id]->m_isVaild = FALSE;
			}
		}

		PacketSCDamage damagePacket;
		makePacketSCDamage(&damagePacket, pSession->m_Id, player.m_Id, player.m_Hp);
		g_NetworkMgr->RegisterBroadcast(nullptr, (char *)&damagePacket, sizeof(PacketSCDamage));
	}

	// 공격 패킷 반송
	PacketSCAttack1 attackPacket;
	makePacketSCAttack1(&attackPacket, pSession->m_Id, packet->direction, packet->x, packet->y);
	g_NetworkMgr->RegisterBroadcast(nullptr, (char *)&attackPacket, sizeof(PacketSCAttack1));

	return true;
}

bool ProcessPacket::PacketProc_Attack2(Session *pSession, char *pPacket)
{
	PacketCSAttack2 *packet = (PacketCSAttack2 *)pPacket;

	wprintf(L"Attack2 # Player Id %d # x : %d, y : %d ", pSession->m_Id, packet->x, packet->y);
	wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id].m_X, g_Players[pSession->m_Id].m_Y);

	MOVE_DIR eDir = (MOVE_DIR)packet->direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	for (auto &p : g_Players)
	{
		Player &player = p.second;

		if (player.m_Id == pSession->m_Id)
			continue;

		int dX = (player.m_X - packet->x) * dir;
		int dY = abs(player.m_Y - packet->y);
		// 음수면 안 맞은 것
		if (dX < 0)
			continue;

		if (!(dX < ATTACK2_RANGE_X && dY < ATTACK2_RANGE_Y))
			continue;

		player.m_Hp -= ATTACK2_DAMAGE;

		if (player.m_Hp <= 0)
		{
			if (g_Sessions[player.m_Id]->m_isVaild)
			{
				g_NetworkMgr->deleteQueue.push_back(g_Sessions[player.m_Id]);
				g_Sessions[player.m_Id]->m_isVaild = FALSE;
			}
		}

		PacketSCDamage damagePacket;
		makePacketSCDamage(&damagePacket, pSession->m_Id, player.m_Id, player.m_Hp);
		g_NetworkMgr->RegisterBroadcast(nullptr, (char *)&damagePacket, sizeof(PacketSCDamage));
	}

	// 공격 패킷 반송
	PacketSCAttack2 attackPacket;
	makePacketSCAttack2(&attackPacket, pSession->m_Id, packet->direction, packet->x, packet->y);
	g_NetworkMgr->RegisterBroadcast(nullptr, (char *)&attackPacket, sizeof(PacketSCAttack2));

	return true;
}

bool ProcessPacket::PacketProc_Attack3(Session *pSession, char *pPacket)
{
	PacketCSAttack3 *packet = (PacketCSAttack3 *)pPacket;

	wprintf(L"Attack3 # Player Id %d # x : %d, y : %d ", pSession->m_Id, packet->x, packet->y);
	wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id].m_X, g_Players[pSession->m_Id].m_Y);

	MOVE_DIR eDir = (MOVE_DIR)packet->direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	for (auto &p : g_Players)
	{
		Player &player = p.second;

		if (player.m_Id == pSession->m_Id)
			continue;

		int dX = (player.m_X - packet->x) * dir;
		int dY = abs(player.m_Y - packet->y);

		// 음수면 안 맞은 것
		if (dX < 0)
			continue;

		// 범위 밖이라면
		if (!(dX < ATTACK3_RANGE_X && dY < ATTACK3_RANGE_Y))
			continue;

		player.m_Hp -= ATTACK3_DAMAGE;

		if (player.m_Hp <= 0)
		{
			if (g_Sessions[player.m_Id]->m_isVaild)
			{
				g_NetworkMgr->deleteQueue.push_back(g_Sessions[player.m_Id]);
				g_Sessions[player.m_Id]->m_isVaild = FALSE;
			}
		}

		PacketSCDamage damagePacket;
		makePacketSCDamage(&damagePacket, pSession->m_Id, player.m_Id, player.m_Hp);
		g_NetworkMgr->RegisterBroadcast(nullptr, (char *)&damagePacket, sizeof(PacketSCDamage));
	}

	// 공격 패킷 반송
	PacketSCAttack3 attackPacket;
	makePacketSCAttack3(&attackPacket, pSession->m_Id, packet->direction, packet->x, packet->y);
	g_NetworkMgr->RegisterBroadcast(nullptr, (char *)&attackPacket, sizeof(PacketSCAttack3));

	return true;
}

bool ProcessPacket::PacketProcCSMoveStart(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	SerializableBuffer sBuffer;
	int ret = pSession->recvBuffer.Dequeue((char *)sBuffer.GetBufferPtr(), header.bySize);
	sBuffer.MoveWritePos(ret);

	INT id;
	CHAR moveDir;
	USHORT x;
	USHORT y;
	BYTE hp;

	sBuffer >> id >> moveDir >> x >> y >> hp;

	wprintf(L"MoveStart # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id].m_X, g_Players[pSession->m_Id].m_Y);

	if (abs(g_Players[pSession->m_Id].m_X - x) > ERROR_RANGE ||
		abs(g_Players[pSession->m_Id].m_Y - y) > ERROR_RANGE)
	{
		return false;
	}

	g_Players[pSession->m_Id].m_Action = (DWORD)moveDir;

	switch ((MOVE_DIR)moveDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		g_Players[pSession->m_Id].m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		g_Players[pSession->m_Id].m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	g_Players[pSession->m_Id].m_X = x;
	g_Players[pSession->m_Id].m_Y = y;

	{
		GenPacket::makePacketSCMoveStart(pSession, pSession->m_Id, moveDir, x, y);
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)&newPacket, sizeof(PacketSCMoveStart));
	}

	return true;
}

bool ProcessPacket::PacketProcCSMoveStop(Session *pSession, PACKET_CODE code)
{
	return false;
}

bool ProcessPacket::PacketProcCSAttack1(Session *pSession, PACKET_CODE code)
{
	return false;
}

bool ProcessPacket::PacketProcCSAttack2(Session *pSession, PACKET_CODE code)
{
	return false;
}

bool ProcessPacket::PacketProcCSAttack3(Session *pSession, PACKET_CODE code)
{
	return false;
}

bool ProcessPacket::PacketProcCSEcho(Session *pSession, PACKET_CODE code)
{
	return false;
}
