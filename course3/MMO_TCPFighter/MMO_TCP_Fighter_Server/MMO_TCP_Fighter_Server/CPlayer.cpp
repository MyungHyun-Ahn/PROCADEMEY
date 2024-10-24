#include "pch.h"
#include "Define.h"
#include "Sector.h"
#include "CSession.h"
#include "GenPacket.h"
#include "GameLogic.h"
#include "CPlayer.h"

unsigned int g_UserId = 0;
std::unordered_map<INT, CPlayer *> g_Players;
CObjectPool<CPlayer> g_PlayerPool(7500, true);

bool CPlayer::Move(SHORT x, SHORT y)
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

		// ����� ���� ���ϱ�
		int nowSecY = CalSectorY(m_Y);
		int nowSecX = CalSectorX(m_X);

		if (nowSecX != m_SecX || nowSecY != m_SecY)
		{
			// ���� �̵��ϸ� Delete Create �޽��� ������
			MoveSector(m_SecY, m_SecX, nowSecY, nowSecX);
		}

		return true;
	}

	return false;
}

void CPlayer::MoveSector(int prevY, int prevX, int nowY, int nowX)
{
	// Player ���� �̵�
	// ���� ���Ϳ��� -> ���� ����

	g_Sectors[prevY][prevX].erase(m_Id);
	g_Sectors[nowY][nowX].insert(std::make_pair(m_Id, this));
	m_SecY = nowY;
	m_SecX = nowX;

	// ���� �̵� - Y ���� ����
	int dX = nowX - prevX;
	int dY = nowY - prevY;

	// ��ȭ���� 1���� ū ���
	if (abs(dX) > 1 || abs(dY) > 1)
	{
		g_Logger->WriteLog(L"SECTOR", LOG_LEVEL::SYSTEM, L"[%04d][SECTOR] PlayerId : %d, Prev X : %d, Y : %d | Now X : %d, Y : %d", m_Id, prevX, prevY, nowX, nowY);

		// prevX, Y �ֺ� ���Ϳ� �÷��̾� ���� �޽���
		// �����׵� �ű� �ֺ� ���� �÷��̾� ���� �޽��� ��������
		{
			int startY = prevY - SECTOR_VIEW_START;
			int startX = prevX - SECTOR_VIEW_START;

			for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
			{
				for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
				{
					if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
						continue;

					for (auto &player : g_Sectors[startY + y][startX + x])
					{
						if (player.second->m_Id == m_Id)
							continue;

						GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[player.first], m_Id);
						GenPacket::makePacketSCDeleteCharacter(FALSE, g_Sessions[m_Id], player.first);
					}
				}
			}
		}

		CPlayer *player = g_Players[m_Id];
		// nowX, Y �ֺ� ���Ϳ� �÷��̾� ���� �޽���
		// �ش� ������ �÷��̾� ������ ���������
		{
			int startY = m_SecY - SECTOR_VIEW_START;
			int startX = m_SecX - SECTOR_VIEW_START;

			for (int y = 0; y < SECTOR_VIEW_COUNT; y++)
			{
				for (int x = 0; x < SECTOR_VIEW_COUNT; x++)
				{
					if (startY + y < 0 || startY + y >= SECTOR_MAX_Y || startX + x < 0 || startX + x >= SECTOR_MAX_X)
						continue;

					for (auto &player : g_Sectors[startY + y][startX + x])
					{
						if (player.second->m_Id == m_Id)
							continue;

						GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[player.first], m_Id, m_Direction, m_X, m_Y, m_Hp);
						if (m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
							GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[player.first], m_Id, (CHAR)m_Action, m_X, m_Y);

						GenPacket::makePacketSCCreateOtherCharacter(FALSE, g_Sessions[m_Id], player.first, m_Direction, player.second->m_X, player.second->m_Y, player.second->m_Hp);
						if (player.second->m_Action != (CHAR)MOVE_DIR::MOVE_DIR_STOP)
							GenPacket::makePacketSCMoveStart(FALSE, g_Sessions[m_Id], player.first, (CHAR)player.second->m_Action, player.second->m_X, player.second->m_Y);
					}
				}
			}
		}

		return;
	}

	int createPlayerX = nowX + SECTOR_VIEW_START * dX;
	int deletePlayerX = prevX + SECTOR_VIEW_START * dX * -1;

	int createPlayerY = nowY + SECTOR_VIEW_START * dY;
	int deletePlayerY = prevY + SECTOR_VIEW_START * dY * -1;

	if (prevY == nowY)
	{
		for (int y = 0; y < 3; y++)
		{
			// ���� �޽���
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

			// ���� �޽���
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
		}
	}
	// ���� �̵� - X ���� ����
	else if (prevX == nowX)
	{
		for (int x = 0; x < 3; x++)
		{
			// ���� �޽���
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

			// ���� �޽���
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
		}
	}
	else // �밢�� �̵�
	{
		// �������� 2�� �̵���Ű��
		MoveSector(prevY, prevX, prevY, nowX);
		MoveSector(prevY, nowX, nowY, nowX);
	}
}
