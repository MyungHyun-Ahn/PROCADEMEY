#include "pch.h"
#include "DefinePacket.h"
#include "Session.h"
#include "ProcessPacket.h"

bool ProcessPacket::PacketProcCSCreateMyCharacter(Session *session)
{

	PacketHeader header;
	session->recvBuffer.Dequeue((char *)&header, sizeof(PacketHeader));
	SerializableBuffer sBuffer;
	session->recvBuffer.Dequeue(sBuffer.GetBufferPtr, header.bySize);

	

	return true;
}
