#pragma once
class GenPacket
{
public:
{%- for pkt in scList %}
	void makePacket{{pkt.name}}(Session *session, {%- for arg in pkt.argList %} {{arg.type}} {{arg.name}}{{'' if loop.last else ','}}{%- endfor %});{{'' if loop.last else '\n'}}
{%- endfor %}
};

