#pragma once
class GenPacket
{
public:
{%- for pkt in scList %}
	static void makePacket{{pkt.name}}(BOOL bIsBroadcast, Session *pSession, {%- for arg in pkt.argList %} {{arg.type}} {{arg.name}}{{'' if loop.last else ','}}{%- endfor %});{{'' if loop.last else '\n'}}
{%- endfor %}
};

