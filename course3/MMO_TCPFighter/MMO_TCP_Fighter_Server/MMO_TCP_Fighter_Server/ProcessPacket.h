#pragma once
class ProcessPacketInterface
{
public:
	virtual bool Process(int sessionId) = 0;
	virtual bool ConsumePacket(CSession *pSession, PACKET_CODE code) = 0;
	virtual bool PacketProcCSMoveStart(CSession *pSession, PACKET_CODE code) = 0;
	virtual bool PacketProcCSMoveStop(CSession *pSession, PACKET_CODE code) = 0;
	virtual bool PacketProcCSAttack1(CSession *pSession, PACKET_CODE code) = 0;
	virtual bool PacketProcCSAttack2(CSession *pSession, PACKET_CODE code) = 0;
	virtual bool PacketProcCSAttack3(CSession *pSession, PACKET_CODE code) = 0;
	virtual bool PacketProcCSEcho(CSession *pSession, PACKET_CODE code) = 0;
};

class ProcessPacket : public ProcessPacketInterface
{
public:
	bool Process(int sessionId)
	{
		CSession *curSession = g_Sessions[sessionId];

		while (true)
		{
			int size = curSession->recvBuffer.GetUseSize();
			if (size < sizeof(PacketHeader))
				break;

			PacketHeader header;
			int ret = curSession->recvBuffer.Peek((char *)&header, sizeof(PacketHeader));
			if (size < header.bySize + sizeof(PacketHeader))
				break;
			
			if (!ConsumePacket(curSession, (PACKET_CODE)header.byType))
			{
				return false;
			}

		}

		return true;
	}

	bool ConsumePacket(CSession *pSession, PACKET_CODE code)
	{
		switch (code)
		{
		case PACKET_CODE::CSMoveStart:
			return PacketProcCSMoveStart(pSession, code);
		case PACKET_CODE::CSMoveStop:
			return PacketProcCSMoveStop(pSession, code);
		case PACKET_CODE::CSAttack1:
			return PacketProcCSAttack1(pSession, code);
		case PACKET_CODE::CSAttack2:
			return PacketProcCSAttack2(pSession, code);
		case PACKET_CODE::CSAttack3:
			return PacketProcCSAttack3(pSession, code);
		case PACKET_CODE::CSEcho:
			return PacketProcCSEcho(pSession, code);
		default:
			break;
		}

		return false;
	}
	bool PacketProcCSMoveStart(CSession *pSession, PACKET_CODE code);
	bool PacketProcCSMoveStop(CSession *pSession, PACKET_CODE code);
	bool PacketProcCSAttack1(CSession *pSession, PACKET_CODE code);
	bool PacketProcCSAttack2(CSession *pSession, PACKET_CODE code);
	bool PacketProcCSAttack3(CSession *pSession, PACKET_CODE code);
	bool PacketProcCSEcho(CSession *pSession, PACKET_CODE code);
};

extern ProcessPacket g_ProcessPacket;
extern ProcessPacketInterface *g_pProcessPacket;