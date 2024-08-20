#include "pch.h"
#include "Define.h"
#include "DefinePacket.h"
#include "Session.h"
#include "Player.h"
#include "NetworkManager.h"
#include "GenPacket.h"
#include "Sector.h"
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

	Player *player = g_Players[pSession->m_Id];
	player->syncList.push_back({ TRUE, player->m_X, player->m_Y, x, y });

	if (abs(player->m_X - x) > ERROR_RANGE ||
		abs(player->m_Y - y) > ERROR_RANGE)
	{
		GenPacket::makePacketSCSync(FALSE, pSession, pSession->m_Id, player->m_X, player->m_Y);
		g_SyncCount++;
		g_Logger->WriteLog(L"SYNC", LOG_LEVEL::SYSTEM, L"[%04d][START] PlayerId : %d, Server X : %d, Y : %d | Client X : %d, Y : %d", g_SyncCount, pSession->m_Id, player->m_X, player->m_Y, x, y);

		for (auto &dbg : player->syncList)
		{
			if (dbg.isStart)
				g_Logger->WriteLog(L"SYNC_PLAYER_HISTORY", LOG_LEVEL::DEBUG, L"[%04d][START] PlayerId : %d, Server X : %d, Y : %d, Client X : %d, Y : %d", g_SyncCount, player->m_Id, dbg.sX, dbg.sY, dbg.cX, dbg.cY);
			else
				g_Logger->WriteLog(L"SYNC_PLAYER_HISTORY", LOG_LEVEL::DEBUG, L"[%04d][STOP] PlayerId : %d, Server X : %d, Y : %d, Client X : %d, Y : %d", g_SyncCount, player->m_Id, dbg.sX, dbg.sY, dbg.cX, dbg.cY);
		}

		// 서버 좌표로 조정
		x = player->m_X;
		y = player->m_Y;
	}

	player->m_Action = (DWORD)moveDir;
	player->m_X = x;
	player->m_Y = y;

	switch ((MOVE_DIR)moveDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		player->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		player->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	int nowSecY = CalSectorY(player->m_Y);
	int nowSecX = CalSectorX(player->m_X);

	if (nowSecX != player->m_SecX || nowSecY != player->m_SecY)
	{
		// 섹터 이동하며 Delete Create 메시지 보내기
		player->MoveSector(player->m_SecY, player->m_SecX, nowSecY, nowSecX);
	}

	// Broadcast
	GenPacket::makePacketSCMoveStart(TRUE, pSession, pSession->m_Id, player->m_Direction, x, y);
	GenPacket::makePacketSCMoveStart(FALSE, pSession, pSession->m_Id, player->m_Direction, x, y);
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

	Player *player = g_Players[pSession->m_Id];
	player->syncList.push_back({ FALSE, player->m_X, player->m_Y, x, y });

	if (abs(player->m_X - x) > ERROR_RANGE ||
		abs(player->m_Y - y) > ERROR_RANGE)
	{
		GenPacket::makePacketSCSync(FALSE, pSession, pSession->m_Id, player->m_X, player->m_Y);

		g_SyncCount++;
		g_Logger->WriteLog(L"SYNC", LOG_LEVEL::SYSTEM, L"[%04d][STOP] PlayerId : %d, Server X : %d, Y : %d | Client X : %d, Y : %d", g_SyncCount, pSession->m_Id, player->m_X, player->m_Y, x, y);

		for (auto &dbg : player->syncList)
		{
			if (dbg.isStart)
				g_Logger->WriteLog(L"SYNC_PLAYER_HISTORY", LOG_LEVEL::DEBUG, L"[%04d][START] PlayerId : %d, Server X : %d, Y : %d, Client X : %d, Y : %d", g_SyncCount, player->m_Id, dbg.sX, dbg.sY, dbg.cX, dbg.cY);
			else
				g_Logger->WriteLog(L"SYNC_PLAYER_HISTORY", LOG_LEVEL::DEBUG, L"[%04d][STOP] PlayerId : %d, , Server X : %d, Y : %d, Client X : %d, Y : %d", g_SyncCount, player->m_Id, dbg.sX, dbg.sY, dbg.cX, dbg.cY);
		}

		// 서버 좌표로 조정
		x = player->m_X;
		y = player->m_Y;
	}

	player->m_Action = (DWORD)MOVE_DIR::MOVE_DIR_STOP;
	player->m_X = x;
	player->m_Y = y;

	switch ((MOVE_DIR)viewDir)
	{
	case MOVE_DIR::MOVE_DIR_RR:
	case MOVE_DIR::MOVE_DIR_RU:
	case MOVE_DIR::MOVE_DIR_RD:
		player->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_RR;
		break;

	case MOVE_DIR::MOVE_DIR_LU:
	case MOVE_DIR::MOVE_DIR_LL:
	case MOVE_DIR::MOVE_DIR_LD:
		player->m_Direction = (BYTE)MOVE_DIR::MOVE_DIR_LL;
		break;
	}

	int nowSecY = CalSectorY(player->m_Y);
	int nowSecX = CalSectorX(player->m_X);

	if (nowSecX != player->m_SecX || nowSecY != player->m_SecY)
	{
		// 섹터 이동하며 Delete Create 메시지 보내기
		player->MoveSector(player->m_SecY, player->m_SecX, nowSecY, nowSecX);
	}

	// Broadcast
	GenPacket::makePacketSCMoveStop(TRUE, pSession, pSession->m_Id, g_Players[pSession->m_Id]->m_Direction, x, y);
	GenPacket::makePacketSCMoveStop(FALSE, pSession, pSession->m_Id, g_Players[pSession->m_Id]->m_Direction, x, y);
	return true;
}

bool ProcessPacket::PacketProcCSAttack1(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR direction;
	USHORT playerX;
	USHORT playerY;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> direction >> playerX >> playerY;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"Attack1 # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	MOVE_DIR eDir = (MOVE_DIR)direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	int startY = g_Players[pSession->m_Id]->m_SecY - SECTOR_VIEW_START;
	int startX = g_Players[pSession->m_Id]->m_SecX - SECTOR_VIEW_START;

	for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
	{
		for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
		{
			if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
				continue;

			for (auto &player : g_Sectors[startY + y][startX + x])
			{
				Player *targetPlayer = player.second;

				if (targetPlayer->m_Id == pSession->m_Id)
					continue;

				int dX = (targetPlayer->m_X - playerX) * dir;
				int dY = abs(targetPlayer->m_Y - playerY);
				// 음수면 안 맞은 것
				if (dX < 0)
					continue;

				if (!(dX < ATTACK1_RANGE_X && dY < ATTACK1_RANGE_Y))
					continue;

				targetPlayer->m_Hp -= ATTACK1_DAMAGE;

				if (targetPlayer->m_Hp <= 0)
				{
					if (g_Sessions[targetPlayer->m_Id]->m_isVaild)
					{
						g_NetworkMgr->deleteQueue.push_back(g_Sessions[targetPlayer->m_Id]);
						g_Sessions[targetPlayer->m_Id]->m_isVaild = FALSE;
					}
				}

				// Broadcast
				GenPacket::makePacketSCDamage(TRUE, pSession, pSession->m_Id, targetPlayer->m_Id, targetPlayer->m_Hp);
				GenPacket::makePacketSCDamage(FALSE, pSession, pSession->m_Id, targetPlayer->m_Id, targetPlayer->m_Hp);
				
				goto END1;
			}
		}
	}

END1:

	// 공격 패킷 반송
	// Broadcast
	GenPacket::makePacketSCAttack1(TRUE, pSession, pSession->m_Id, direction, playerX, playerY);
	GenPacket::makePacketSCAttack1(FALSE, pSession, pSession->m_Id, direction, playerX, playerY);

	return true;
}

bool ProcessPacket::PacketProcCSAttack2(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR direction;
	USHORT playerX;
	USHORT playerY;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);

		*sBuffer >> direction >> playerX >> playerY;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"Attack2 # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	MOVE_DIR eDir = (MOVE_DIR)direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	int startY = g_Players[pSession->m_Id]->m_SecY - SECTOR_VIEW_START;
	int startX = g_Players[pSession->m_Id]->m_SecX - SECTOR_VIEW_START;

	for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
	{
		for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
		{
			if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
				continue;

			auto &s = g_Sectors[startY + y][startX + x];
			for (auto &player : g_Sectors[startY + y][startX + x])
			{
				Player *targetPlayer = player.second;

				if (targetPlayer->m_Id == pSession->m_Id)
					continue;

				int dX = (targetPlayer->m_X - playerX) * dir;
				int dY = abs(targetPlayer->m_Y - playerY);
				// 음수면 안 맞은 것
				if (dX < 0)
					continue;

				if (!(dX < ATTACK2_RANGE_X && dY < ATTACK2_RANGE_Y))
					continue;

				targetPlayer->m_Hp -= ATTACK2_DAMAGE;

				if (targetPlayer->m_Hp <= 0)
				{
					if (g_Sessions[targetPlayer->m_Id]->m_isVaild)
					{
						g_NetworkMgr->deleteQueue.push_back(g_Sessions[targetPlayer->m_Id]);
						g_Sessions[targetPlayer->m_Id]->m_isVaild = FALSE;
					}
				}

				// Broadcast
				GenPacket::makePacketSCDamage(TRUE, pSession, pSession->m_Id, targetPlayer->m_Id, targetPlayer->m_Hp);
				GenPacket::makePacketSCDamage(FALSE, pSession, pSession->m_Id, targetPlayer->m_Id, targetPlayer->m_Hp);

				goto END2;
			}
		}
	}

END2:
	// 공격 패킷 반송
	// Broadcast
	GenPacket::makePacketSCAttack2(TRUE, pSession, pSession->m_Id, direction, playerX, playerY);
	GenPacket::makePacketSCAttack2(FALSE, pSession, pSession->m_Id, direction, playerX, playerY);

	return true;
}

bool ProcessPacket::PacketProcCSAttack3(Session *pSession, PACKET_CODE code)
{
	PacketHeader header;
	pSession->recvBuffer.Dequeue((char *)&header, sizeof(header));

	CHAR direction;
	USHORT playerX;
	USHORT playerY;

	{
		SerializableBuffer *sBuffer = g_SBufferPool.Alloc();
		int ret = pSession->recvBuffer.Dequeue((char *)sBuffer->GetBufferPtr(), header.bySize);
		sBuffer->MoveWritePos(ret);
		*sBuffer >> direction >> playerX >> playerY;
		sBuffer->Clear();
		g_SBufferPool.Free(sBuffer);
	}

	// wprintf(L"Attack3 # Player Id %d # x : %d, y : %d ", pSession->m_Id, x, y);
	// wprintf(L"# curX : %d, curY : %d\n", g_Players[pSession->m_Id]->m_X, g_Players[pSession->m_Id]->m_Y);

	MOVE_DIR eDir = (MOVE_DIR)direction;
	int dir = eDir == MOVE_DIR::MOVE_DIR_RR ? 1 : -1;

	int startY = g_Players[pSession->m_Id]->m_SecY - SECTOR_VIEW_START;
	int startX = g_Players[pSession->m_Id]->m_SecX - SECTOR_VIEW_START;

	for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
	{
		for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
		{
			if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
				continue;

			auto &s = g_Sectors[startY + y][startX + x];
			for (auto &player : g_Sectors[startY + y][startX + x])
			{
				Player *targetPlayer = player.second;

				if (targetPlayer->m_Id == pSession->m_Id)
					continue;

				int dX = (targetPlayer->m_X - playerX) * dir;
				int dY = abs(targetPlayer->m_Y - playerY);
				// 음수면 안 맞은 것
				if (dX < 0)
					continue;

				if (!(dX < ATTACK3_RANGE_X && dY < ATTACK3_RANGE_Y))
					continue;

				targetPlayer->m_Hp -= ATTACK3_DAMAGE;

				if (targetPlayer->m_Hp <= 0)
				{
					if (g_Sessions[targetPlayer->m_Id]->m_isVaild)
					{
						g_NetworkMgr->deleteQueue.push_back(g_Sessions[targetPlayer->m_Id]);
						g_Sessions[targetPlayer->m_Id]->m_isVaild = FALSE;
					}
				}

				// Broadcast
				GenPacket::makePacketSCDamage(TRUE, pSession, pSession->m_Id, targetPlayer->m_Id, targetPlayer->m_Hp);
				GenPacket::makePacketSCDamage(FALSE, pSession, pSession->m_Id, targetPlayer->m_Id, targetPlayer->m_Hp);

				goto END3;
			}
		}
	}

END3:
	// 공격 패킷 반송
	// Broadcast
	GenPacket::makePacketSCAttack3(TRUE, pSession, pSession->m_Id, direction, playerX, playerY);
	GenPacket::makePacketSCAttack3(FALSE, pSession, pSession->m_Id, direction, playerX, playerY);

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
