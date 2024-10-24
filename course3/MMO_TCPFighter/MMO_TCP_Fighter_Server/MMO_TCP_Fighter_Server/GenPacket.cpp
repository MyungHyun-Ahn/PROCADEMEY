#include "pch.h"
#include "DefinePacket.h"
#include "CSession.h"
#include "GenPacket.h"
#include "Define.h"
#include "CNetworkManager.h"

CSerializableBuffer &operator<<(CSerializableBuffer &sBuffer, PacketHeader &header)
{
	sBuffer << header.byCode << header.bySize << header.byType;
	return sBuffer;
}
void GenPacket::makePacketSCCreateMyCharacter(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT) +  sizeof(BYTE);
	header.byType = (BYTE)PACKET_CODE::SCCreateMyCharacter;

	buffer << header << id <<  direction <<  x <<  y <<  hp;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCCreateOtherCharacter(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT) +  sizeof(BYTE);
	header.byType = (BYTE)PACKET_CODE::SCCreateOtherCharacter;

	buffer << header << id <<  direction <<  x <<  y <<  hp;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCDeleteCharacter(BOOL bIsBroadcast, CSession *pSession, INT id)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT);
	header.byType = (BYTE)PACKET_CODE::SCDeleteCharacter;

	buffer << header << id;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCMoveStart(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCMoveStart;

	buffer << header << id <<  direction <<  x <<  y;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCMoveStop(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCMoveStop;

	buffer << header << id <<  direction <<  x <<  y;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCAttack1(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCAttack1;

	buffer << header << id <<  direction <<  x <<  y;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCAttack2(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCAttack2;

	buffer << header << id <<  direction <<  x <<  y;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCAttack3(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(CHAR) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCAttack3;

	buffer << header << id <<  direction <<  x <<  y;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCDamage(BOOL bIsBroadcast, CSession *pSession, INT attackId, INT damageId, CHAR damageHp)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(INT) +  sizeof(CHAR);
	header.byType = (BYTE)PACKET_CODE::SCDamage;

	buffer << header << attackId <<  damageId <<  damageHp;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCSync(BOOL bIsBroadcast, CSession *pSession, INT id, USHORT x, USHORT y)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(INT) +  sizeof(USHORT) +  sizeof(USHORT);
	header.byType = (BYTE)PACKET_CODE::SCSync;

	buffer << header << id <<  x <<  y;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 

void GenPacket::makePacketSCEcho(BOOL bIsBroadcast, CSession *pSession, DWORD time)
{
	CSerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = sizeof(DWORD);
	header.byType = (BYTE)PACKET_CODE::SCEcho;

	buffer << header << time;

	if (bIsBroadcast)
		g_NetworkMgr->RegisterBroadcast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
	else
		g_NetworkMgr->RegisterUnicast(pSession, (char *)buffer.GetBufferPtr(), buffer.GetDataSize());
} 