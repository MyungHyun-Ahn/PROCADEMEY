#pragma once
class ProcessPacketInterface
{
public:
	virtual bool Process(int sessionId) = 0;
	virtual bool ConsumePacket(CSession *pSession, PACKET_CODE code) = 0;

	{%- for pkt in csList %}
	virtual bool PacketProc{{pkt.name}}(CSession *pSession, PACKET_CODE code) = 0;
	{%- endfor %}
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
		{%- for pkt in csList %}
		case PACKET_CODE::{{pkt.name}}:
			return PacketProc{{pkt.name}}(pSession, code);
		{%- endfor %}
		default:
			break;
		}

		return false;
	}

	{%- for pkt in csList %}
	bool PacketProc{{pkt.name}}(CSession *pSession, PACKET_CODE code);
	{%- endfor %}
};

extern ProcessPacket g_ProcessPacket;
extern ProcessPacketInterface *g_pProcessPacket;