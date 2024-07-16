#include "pch.h"
#include "DefinePacket.h"
#include "Session.h"
#include "GenPacket.h"

{%- for pkt in scList %}
void GenPacket::makePacket{{pkt.name}}(Session *session, {%- for arg in pkt.argList %} {{arg.type}} {{arg.name}}{{'' if loop.last else ','}}{%- endfor %})
{
	SerializableBuffer buffer;
	PacketHeader header;
	
	header.byCode = dfPACKET_CODE;
	header.bySize = {%- for arg in pkt.argList %} {{'sizeof('}}{{arg.type}}{{')'}}{{';' if loop.last else ' + '}}{%- endfor %}
	header.byType = (BYTE)PACKET_CODE::{{pkt.name}};

	buffer << header << {%- for arg in pkt.argList %} {{arg.name}}{{';' if loop.last else ' << '}}{%- endfor %}
	g_NetworkMgr->RegisterUnicast(pSession, (char *)&buffer, buffer.GetDataSize);
} {{'' if loop.last else '\n'}}
{%- endfor %}
