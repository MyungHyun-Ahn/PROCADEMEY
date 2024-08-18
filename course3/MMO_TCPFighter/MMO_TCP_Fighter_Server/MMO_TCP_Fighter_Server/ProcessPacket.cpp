#include "pch.h"
#include "Define.h"
#include "DefinePacket.h"
#include "Session.h"
#include "Player.h"
#include "NetworkManager.h"
#include "GenPacket.h"
#include "ProcessPacket.h"

ProcessPacket g_ProcessPacket;
ProcessPacketInterface *g_pProcessPacket = &g_ProcessPacket;

SerializableBuffer &operator>>(SerializableBuffer &sBuffer, PacketHeader &header)
{
	sBuffer.Dequeue((char *)&header, sizeof(header));
	return sBuffer;
}

bool ProcessPacket::PacketProcCSMoveStart(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR moveDir;
	USHORT x;
	USHORT y;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> moveDir >> x >> y;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"MoveStart # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	if (abs(g_Players[pSession->m_Id]->m_X - x) > ERROR_RANGE ||
		abs(g_Players[pSession->m_Id]->m_Y - y) > ERROR_RANGE)
	{
		GenPacket::makePacketSCSync(FALSE, pSession, pSession->m_Id, g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);
	}

	g_Players[pSession->m_Id]->m_Action = (DWORD)moveDir;

	switch ((MOVE_DIR)moveDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		g_Players[pSession->m_Id]->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		g_Players[pSession->m_Id]->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	// Broadcast
	GenPacket::makePacketSCMoveStart(TRUE, pSession, pSession->m_Id, moveDir, g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);
	return true;
}

bool ProcessPacket::PacketProcCSMoveStop(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR viewDir;
	USHORT x;
	USHORT y;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> viewDir >> x >> y;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"MoveStop # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	if (abs(g_Players[pSession->m_Id]->m_X - x) > ERROR_RANGE ||
		abs(g_Players[pSession->m_Id]->m_Y - y) > ERROR_RANGE)
	{
		GenPacket::makePacketSCSync(FALSE, pSession, pSession->m_Id, g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);
	}

	g_Players[pSession->m_Id]->m_Action = (DWORD)MOVE_DIR::MOVE_DIR_STOP;

	switch ((MOVE_DIR)viewDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		g_Players[pSession->m_Id]->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		g_Players[pSession->m_Id]->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	// Broadcast
	GenPacket::makePacketSCMoveStop(TRUE, pSession, pSession->m_Id, g_Players[pSession->m_Id]->m_Direction, g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);
	return true;
}

bool ProcessPacket::PacketProcCSAttack1(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR direction;
	USHORT x;
	USHORT y;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> direction >> x >> y;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"Attack1 # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	MOVE_DIR eDir = (MOVE_DIR)direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	for (auto &p : g_Players)
	{
		Player *player = p.second;

		if (player->m_Id == pSession->m_Id)
			continue;

		int dX = (player->m_X - x) * dir;
		int dY = abs(player->m_Y - y);
		// 음수면 안 맞은 것
		if (dX < 0)
			continue;

		if (!(dX < ATTACK1_RANGE_X && dY < ATTACK1_RANGE_Y))
			continue;

		player->m_Hp -= ATTACK1_DAMAGE;

		if (player->m_Hp <= 0)
		{
			if (g_Sessions[player->m_Id]->m_isVaild)
			{
				g_NetworkMgr->deleteQueue.push_back(g_Sessions[player->m_Id]);
				g_Sessions[player->m_Id]->m_isVaild = FALSE;
			}
		}

		// Broadcast
		GenPacket::makePacketSCDamage(TRUE, pSession, pSession->m_Id, player->m_Id, player->m_Hp);
		GenPacket::makePacketSCDamage(FALSE, pSession, pSession->m_Id, player->m_Id, player->m_Hp);
	}

	// 공격 패킷 반송
	// Broadcast
	GenPacket::makePacketSCAttack1(TRUE, pSession, pSession->m_Id, direction, x, y);
	GenPacket::makePacketSCAttack1(FALSE, pSession, pSession->m_Id, direction, x, y);

	return true;
}

bool ProcessPacket::PacketProcCSAttack2(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR direction;
	USHORT x;
	USHORT y;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);

		*sBuffer >> direction >> x >> y;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"Attack2 # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	MOVE_DIR eDir = (MOVE_DIR)direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	for (auto &p : g_Players)
	{
		Player *player = p.second;

		if (player->m_Id == pSession->m_Id)
			continue;

		int dX = (player->m_X - x) * dir;
		int dY = abs(player->m_Y - y);
		// 음수면 안 맞은 것
		if (dX < 0)
			continue;

		if (!(dX < ATTACK2_RANGE_X && dY < ATTACK2_RANGE_Y))
			continue;

		player->m_Hp -= ATTACK2_DAMAGE;

		if (player->m_Hp <= 0)
		{
			if (g_Sessions[player->m_Id]->m_isVaild)
			{
				g_NetworkMgr->deleteQueue.push_back(g_Sessions[player->m_Id]);
				g_Sessions[player->m_Id]->m_isVaild = FALSE;
			}
		}

		// Broadcast
		GenPacket::makePacketSCDamage(TRUE, pSession, pSession->m_Id, player->m_Id, player->m_Hp);
		GenPacket::makePacketSCDamage(FALSE, pSession, pSession->m_Id, player->m_Id, player->m_Hp);
	}

	// 공격 패킷 반송
	// Broadcast
	GenPacket::makePacketSCAttack2(TRUE, pSession, pSession->m_Id, direction, x, y);
	GenPacket::makePacketSCAttack2(FALSE, pSession, pSession->m_Id, direction, x, y);

	return true;
}

bool ProcessPacket::PacketProcCSAttack3(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR direction;
	USHORT x;
	USHORT y;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> direction >> x >> y;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"Attack3 # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	MOVE_DIR eDir = (MOVE_DIR)direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	for (auto &p : g_Players)
	{
		Player *player = p.second;

		if (player->m_Id == pSession->m_Id)
			continue;

		int dX = (player->m_X - x) * dir;
		int dY = abs(player->m_Y - y);

		// 음수면 안 맞은 것
		if (dX < 0)
			continue;

		// 범위 밖이라면
		if (!(dX < ATTACK3_RANGE_X && dY < ATTACK3_RANGE_Y))
			continue;

		player->m_Hp -= ATTACK3_DAMAGE;

		if (player->m_Hp <= 0)
		{
			if (g_Sessions[player->m_Id]->m_isVaild)
			{
				g_NetworkMgr->deleteQueue.push_back(g_Sessions[player->m_Id]);
				g_Sessions[player->m_Id]->m_isVaild = FALSE;
			}
		}

		// Broadcast
		GenPacket::makePacketSCDamage(TRUE, pSession, pSession->m_Id, player->m_Id, player->m_Hp);
		GenPacket::makePacketSCDamage(FALSE, pSession, pSession->m_Id, player->m_Id, player->m_Hp);
	}

	// 공격 패킷 반송
	// Broadcast
	GenPacket::makePacketSCAttack3(TRUE, pSession, pSession->m_Id, direction, x, y);
	GenPacket::makePacketSCAttack3(FALSE, pSession, pSession->m_Id, direction, x, y);

	return true;
}

bool ProcessPacket::PacketProcCSEcho(Session *pSession, PACKET_CODE code)
{
	// wprintf(L"Echo # Player Id %d\n", pSession->m_Id);

	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	DWORD time;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> time;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	GenPacket::makePacketSCEcho(FALSE, pSession, time);

	return true;
}
