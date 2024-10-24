#include "pch.h"
#include "DefinePacket.h"
#include "CSession.h"
#include "ProcessPacket.h"

bool ProcessPacket::PacketProcCSCreateMyCharacter(CSession *session)
{

	PacketHeader header;
	session->recvBuffer.Dequeue((char *)&header, sizeof(PacketHeader));
	CSerializableBuffer sBuffer;
	session->recvBuffer.Dequeue(sBuffer.GetBufferPtr, header.bySize);

	

	return true;
}
