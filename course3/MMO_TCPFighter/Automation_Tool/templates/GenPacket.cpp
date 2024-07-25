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

{%- for pkt in scList %}
void GenPacket::makePacket{{pkt.name}}(Session *pSession, {%- for arg in pkt.argList %} {{arg.type}} {{arg.name}}{{'' if loop.last else ','}}{%- endfor %})
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = PACKET_IDENTIFIER;
	header.bySize = {%- for arg in pkt.argList %} {{'sizeof('}}{{arg.type}}{{')'}}{{';' if loop.last else ' + '}}{%- endfor %}
	header.byType = (BYTE)PACKET_CODE::{{pkt.name}};

	buffer << header << {%- for arg in pkt.argList %} {{arg.name}}{{';' if loop.last else ' << '}}{%- endfor %}
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize());
} {{'' if loop.last else '\n'}}
{%- endfor %}
