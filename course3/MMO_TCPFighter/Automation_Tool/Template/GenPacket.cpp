#include "pch.h"
#include "DefinePacket.h"
#include "Session.h"
#include "GenPacket.h"

void GenPacket::makePacketSCCreateMyCharacter(Session *session, INT id, CHAR dir, USHORT x, USHORT y, BYTE hp)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = dfPACKET_CODE;
	header.bySize = sizeof(INT) + sizeof(CHAR) + sizeof(USHORT) + sizeof(USHORT) + sizeof(BYTE);
	header.byType = (BYTE)PACKET_CODE::SCCreateMyCharacter;

	buffer << id << dir << x << y << hp;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize);
}
