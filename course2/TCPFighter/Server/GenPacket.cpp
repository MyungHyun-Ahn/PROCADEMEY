#include "pch.h"
#include "Define.h"
#include "Packet.h"
#include "GenPacket.h"

void makePacketSCCreateMyCharacter(PacketSCCreateMyCharacter *packet, INT id, CHAR dir, USHORT x, USHORT y, BYTE hp)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCCreateMyCharacter) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_CREATE_MY_CHARACTER;

	packet->id = id;
	packet->direction = dir;
	packet->x = x;
	packet->y = y;
	packet->hp = hp;
}

void makePacketSCCreateOtherCharacter(PacketSCCreateOtherCharacter *packet, INT id, CHAR dir, USHORT x, USHORT y, CHAR hp)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCCreateOtherCharacter) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_CREATE_OTHER_CHARACTER;

	packet->id = id;
	packet->direction = dir;
	packet->x = x;
	packet->y = y;
	packet->hp = hp;
}

void makePacketSCDeleteCharacter(PacketSCDeleteCharacter *packet, INT id)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCDeleteCharacter) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_DELETE_CHARACTER;

	packet->id = id;
}

void makePacketSCMoveStart(PacketSCMoveStart *packet, INT id, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCMoveStart) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_MOVE_START;

	packet->id = id;
	packet->direction = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketSCMoveStop(PacketSCMoveStop *packet, INT id, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCMoveStop) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_MOVE_STOP;

	packet->id = id;
	packet->direction = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketSCAttack1(PacketSCAttack1 *packet, INT id, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCAttack1) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_ATTACK1;

	packet->id = id;
	packet->viewDir = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketSCAttack2(PacketSCAttack2 *packet, INT id, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCAttack2) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_ATTACK2;

	packet->id = id;
	packet->viewDir = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketSCAttack3(PacketSCAttack3 *packet, INT id, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCAttack3) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_ATTACK3;

	packet->id = id;
	packet->viewDir = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketSCDamage(PacketSCDamage *packet, INT attacker, INT attacked, CHAR damage)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketSCDamage) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_SC_DAMAGE;

	packet->attackerId = attacker;
	packet->attackedId = attacked;
	packet->hpToReduce = damage;
}



void makePacketCSMoveStart(PacketCSMoveStart *packet, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketCSMoveStart) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_CS_MOVE_START;

	packet->moveDir = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketCSMoveStop(PacketCSMoveStop *packet, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketCSMoveStop) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_CS_MOVE_STOP;

	packet->viewDir = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketCSAttack1(PacketCSAttack1 *packet, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketCSAttack1) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_CS_ATTACK1;

	packet->direction = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketCSAttack2(PacketCSAttack2 *packet, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketCSAttack2) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_CS_ATTACK2;

	packet->direction = dir;
	packet->x = x;
	packet->y = y;
}

void makePacketCSAttack3(PacketCSAttack3 *packet, CHAR dir, USHORT x, USHORT y)
{
	packet->byCode = dfPACKET_CODE;
	packet->bySize = sizeof(PacketCSAttack3) - sizeof(PacketHeader);
	packet->byType = (BYTE)PACKET_CODE::PACKET_CS_ATTACK3;

	packet->direction = dir;
	packet->x = x;
	packet->y = y;
}
