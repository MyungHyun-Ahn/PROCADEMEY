#include "pch.h"
#include "Define.h"
#include "CPlayer.h"
#include "GameLogic.h"

void GameLogic::Update()
{
	for (auto &player : g_Players)
	{
		switch ((MOVE_DIR)player.second->m_Action)
		{
		// ¿ÞÂÊ
		case MOVE_DIR::MOVE_DIR_LL:
			player.second->Move(-SPEED_PLAYER_X, 0);
			break;

		// ´ë°¢¼± ¿ÞÂÊ ¾Æ·¡
		case MOVE_DIR::MOVE_DIR_LU:
			player.second->Move(-SPEED_PLAYER_X, -SPEED_PLAYER_Y);
			break;

		// À§
		case MOVE_DIR::MOVE_DIR_UU:
			player.second->Move(0, -SPEED_PLAYER_Y);
			break;

		case MOVE_DIR::MOVE_DIR_RU:
			player.second->Move(SPEED_PLAYER_X, -SPEED_PLAYER_Y);
			break;

		case MOVE_DIR::MOVE_DIR_RR:
			player.second->Move(SPEED_PLAYER_X, 0);
			break;

		case MOVE_DIR::MOVE_DIR_RD:
			player.second->Move(SPEED_PLAYER_X, SPEED_PLAYER_Y);
			break;

		case MOVE_DIR::MOVE_DIR_DD:
			player.second->Move(0, SPEED_PLAYER_Y);
			break;

		case MOVE_DIR::MOVE_DIR_LD:
			player.second->Move(-SPEED_PLAYER_X, SPEED_PLAYER_Y);
			break;

		default:
			break;
		}
	}
}
