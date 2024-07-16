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
				return false;
			}

		}

		return true;
	}

	bool ConsumePacket(Session *session, PACKET_CODE code)
	{
		switch (code)
		{
		{%- for pkt in csList %}
		case PACKET_CODE::{{pkt.name}}:
			return PacketProc{{pkt.name}}(session, code);
		{%- endfor %}
		default:
			break;
		}
	}

	{%- for pkt in csList %}
	virtual bool PacketProc{{pkt.name}}r(Session *session, PACKET_CODE code) = 0;
	{%- endfor %}
};

class ProcessPacket : public ProcessPacketInterface
{
public:
	{%- for pkt in csList %}
	bool PacketProc{{pkt.name}}(Session *session, PACKET_CODE code);
	{%- endfor %}
};