#pragma once
class ProcessPacketInterface
{
public:
	bool Process(int sessionId)
	{
		Session *curSession = g_Sessions[sessionId];

		while (true)
		{
			int size = curSession->recvBuffer.GetUseSize();
			if (size < sizeof(PacketHeader))
				break;

			PacketHeader header;
			curSession->recvBuffer.Peek((char *)&header, sizeof(PacketHeader));
			if (size < header.bySize + sizeof(PacketHeader))
				break;

			if (!ConsumePacket(curSession, (PACKET_CODE)header.byType))
			{
				// false가 반환 되는 건 잘못된 상황
				return false;
			}

		}

		return true;
	}

	bool ConsumePacket(Session *session, PACKET_CODE code)
	{
		switch (code)
		{
		case PACKET_CODE::CSCreateMyCharacter:
			return PacketProcCSCreateMyCharacter(session, code);

		default:
			break;
		}
	}

	virtual bool PacketProcCSCreateMyCharacter(Session *session, PACKET_CODE code) = 0;
};

class ProcessPacket : public ProcessPacketInterface
{
public:
	bool PacketProcCSCreateMyCharacter(Session *session, PACKET_CODE code);
};