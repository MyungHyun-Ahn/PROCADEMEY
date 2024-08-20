#include "pch.h"
#include "Define.h"
#include "Sector.h"
#include "Session.h"
#include "GenPacket.h"
#include "GameLogic.h"
#include "Player.h"

unsigned int g_UserId = 0;
std::unordered_map<INT, Player *> g_Players;
ObjectPool<Player> g_PlayerPool(7500, true);

bool Player::Move(SHORT x, SHORT y)
{
	bool flagX = true;
	bool flagY = true;

	if (m_X + x < RANGE_MOVE_LEFT || m_X + x >= RANGE_MOVE_RIGHT)
	{
		flagX = false;
	}

	if (m_Y + y < RANGE_MOVE_TOP || m_Y + y >= RANGE_MOVE_BOTTOM)
	{
		flagY = false;
	}

	if (flagX && flagY)
	{
		m_X += x;
		m_Y += y;

		// 변경된 섹터 구하기
		int nowSecY = CalSectorY(m_Y);
		int nowSecX = CalSectorX(m_X);

		if (nowSecX != m_SecX || nowSecY != m_SecY)
		{
			// 섹터 이동하며 Delete Create 메시지 보내기
			MoveSector(m_SecY, m_SecX, nowSecY, nowSecX);
		}

		return true;
	}

	return false;
}

void Player::MoveSector(int prevY, int prevX, int nowY, int nowX)
{
	// Player 섹터 이동
	// 이전 섹터에서 -> 다음 섹터

	g_Sectors[prevY][prevX].erase(m_Id);

	// 가로 이동 - Y 축은 같음
	int dX = nowX - prevX;
	int dY = nowY - prevY;

	int createPlayerX = nowX + SECTOR_VIEW_START * dX;
	int deletePlayerX = prevX + SECTOR_VIEW_START * dX * -1;

	int createPlayerY = nowY + SECTOR_VIEW_START * dY;
	int deletePlayerY = prevY + SECTOR_VIEW_START * dY * -1;

	if (prevY == nowY)
	{
		// wprintf(L"Player id %d Move createPlayerX : %d, deletePlayerX : %d\n", m_Id, createPlayerX, deletePlayerX);

		for (int y = 0; y < 3; y++)
		{
			// 생성 메시지
			do 
			{
				if (createPlayerX < 0 || createPlayerX >= SECTOR_MAX_X)
					break;

				if (nowY + y - 1 < 0 || nowY + y - 1 >= SECTOR_MAX_Y)
					break;

				for (auto player : g_Sectors[nowY + y - 1][createPlayerX])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[player.first], m_Id, m_Direction, m_X, m_Y, m_Hp);
					if (m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[player.first], m_Id, (CHAR)m_Action, m_X, m_Y);

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[m_Id], player.first, m_Direction, player.second->m_X, player.second->m_Y, player.second->m_Hp);
					if (player.second->m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[m_Id], player.first, (CHAR)player.second->m_Action, player.second->m_X, player.second->m_Y);
				}
			} while (0);

			// 삭제 메시지
			do
			{
				if (deletePlayerX < 0 || deletePlayerX >= SECTOR_MAX_X)
					break;

				if (prevY + y - 1 < 0 || prevY + y - 1 >= SECTOR_MAX_Y)
					break;

				for (auto player : g_Sectors[prevY + y - 1][deletePlayerX])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[player.first], m_Id);
					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[m_Id], player.first);
				}
			} while (0);
		}
	}
	// 세로 이동 - X 축은 같음
	else if (prevX == nowX)
	{
		// wprintf(L"Player id %d Move createPlayerX : %d, deletePlayerX : %d\n", m_Id, createPlayerY, deletePlayerY);

		for (int x = 0; x < 3; x++)
		{
			// 생성 메시지
			do 
			{
				if (createPlayerY < 0 || createPlayerY >= SECTOR_MAX_Y)
					break;

				if (nowX + x - 1 < 0 || nowX + x - 1 >= SECTOR_MAX_X)
					break;

				for (auto player : g_Sectors[createPlayerY][nowX + x - 1])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[player.first], m_Id, m_Direction, m_X, m_Y, m_Hp);
					if (m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[player.first], m_Id, (CHAR)m_Action, m_X, m_Y);

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[m_Id], player.first, m_Direction, player.second->m_X, player.second->m_Y, m_Hp);
					if (player.second->m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[m_Id], player.first, (CHAR)player.second->m_Action, player.second->m_X, player.second->m_Y);
				}
			} while (0);

			// 삭제 메시지
			do 
			{
				if (deletePlayerY < 0 || deletePlayerY >= SECTOR_MAX_Y)
					break;

				if (prevX + x - 1 < 0 || prevX + x - 1 >= SECTOR_MAX_X)
					break;

				for (auto player : g_Sectors[deletePlayerY][prevX + x - 1])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[player.first], m_Id);
					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[m_Id], player.first);
				}
			} while (0);
		}
	}
	else // 대각선 이동
	{
		for (int y = 0; y < 3; y++)
		{
			// 생성 메시지
			do
			{
				if (createPlayerX < 0 || createPlayerX >= SECTOR_MAX_X)
					break;

				if (nowY + y - 1 < 0 || nowY + y - 1 >= SECTOR_MAX_Y)
					break;

				for (auto player : g_Sectors[nowY + y - 1][createPlayerX])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[player.first], m_Id, m_Direction, m_X, m_Y, m_Hp);
					if (m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[player.first], m_Id, (CHAR)m_Action, m_X, m_Y);

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[m_Id], player.first, m_Direction, player.second->m_X, player.second->m_Y, player.second->m_Hp);
					if (player.second->m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[m_Id], player.first, (CHAR)player.second->m_Action, player.second->m_X, player.second->m_Y);
				}
			} while (0);

			// 삭제 메시지
			do
			{
				if (deletePlayerX < 0 || deletePlayerX >= SECTOR_MAX_X)
					break;

				if (prevY + y - 1 < 0 || prevY + y - 1 >= SECTOR_MAX_Y)
					break;

				for (auto player : g_Sectors[prevY + y - 1][deletePlayerX])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[player.first], m_Id);
					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[m_Id], player.first);
				}
			} while (0);
		}

		for (int x = 1; x < 3; x++)
		{
			// 생성 메시지
			do
			{
				if (createPlayerY < 0 || createPlayerY >= SECTOR_MAX_Y)
					break;

				if (nowX + x - 1 < 0 || nowX + x - 1 >= SECTOR_MAX_X)
					break;

				for (auto player : g_Sectors[createPlayerY][nowX + x - 1])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[player.first], m_Id, m_Direction, m_X, m_Y, m_Hp);
					if (m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[player.first], m_Id, (CHAR)m_Action, m_X, m_Y);

					GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[m_Id], player.first, m_Direction, player.second->m_X, player.second->m_Y, m_Hp);
					if (player.second->m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
						GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[m_Id], player.first, (CHAR)player.second->m_Action, player.second->m_X, player.second->m_Y);
				}
			} while (0);

			// 삭제 메시지
			do
			{
				if (deletePlayerY < 0 || deletePlayerY >= SECTOR_MAX_Y)
					break;

				if (prevX + x - 1 < 0 || prevX + x - 1 >= SECTOR_MAX_X)
					break;

				for (auto player : g_Sectors[deletePlayerY][prevX + x - 1])
				{
					if (player.first == m_Id)
						continue;

					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[player.first], m_Id);
					GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[m_Id], player.first);
				}
			} while (0);
		}
	}

	g_Sectors[nowY][nowX].insert(std::make_pair(m_Id, this));
	m_SecY = nowY;
	m_SecX = nowX;
}
