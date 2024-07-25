#include "pch.h"
#include "DefinePacket.h"
#include "Session.h"
#include "GenPacket.h"
#include "Define.h"
#include "NetworkManager.h"

SerializableBuffer &operator<<(SerializableBuffer &sBuffer, PacketHeader &header)
{
	sBuffer << header.byCode << header.bySize << header.byType;
	return sBuffer;
}
void GenPacket::makePacketSCCreateMyCharacter(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT) +  sizeof(BYTE);
	header.byType = (BYTE)PACKET_CODE::SCCreateMyCharacter;

	buffer << header << id <<  direction <<  x <<  y <<  hp;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCCreateOtherCharacter(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT) +  sizeof(BYTE);
	header.byType = (BYTE)PACKET_CODE::SCCreateOtherCharacter;

	buffer << header << id <<  direction <<  x <<  y <<  hp;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCDeleteCharacter(Session *pSession, INT id)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT);
	header.byType = (BYTE)PACKET_CODE::SCDeleteCharacter;

	buffer << header << id;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCMoveStart(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCMoveStart;

	buffer << header << id <<  direction <<  x <<  y;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCMoveStop(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCMoveStop;

	buffer << header << id <<  direction <<  x <<  y;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCAttack1(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCAttack1;

	buffer << header << id <<  direction <<  x <<  y;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCAttack2(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCAttack2;

	buffer << header << id <<  direction <<  x <<  y;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCAttack3(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCAttack3;

	buffer << header << id <<  direction <<  x <<  y;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCDamage(Session *pSession, INT attackId, INT damageId, CHAR damageHp)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(INT) +  sizeof(CHAR);
	header.byType = (BYTE)PACKET_CODE::SCDamage;

	buffer << header << attackId <<  damageId <<  damageHp;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCSync(Session *pSession, INT id, USHORT x, USHORT y)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCSync;

	buffer << header << id <<  x <<  y;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 

void GenPacket::makePacketSCEcho(Session *pSession, DWORD time)
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(DWORD);
	header.byType = (BYTE)PACKET_CODE::SCEcho;

	buffer << header << time;
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} 