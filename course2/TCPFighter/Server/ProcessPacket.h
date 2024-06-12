#pragma once

class ProcessPacket
{
public:
	friend class NetworkManager;

	static bool PacketProc_MoveStart(Session *pSession, char *pPacket);

	static bool PacketProc_MoveStop(Session *pSession, char *pPacket);

	static bool PacketProc_Attack1(Session *pSession, char *pPacket);

	static bool PacketProc_Attack2(Session *pSession, char *pPacket);

	static bool PacketProc_Attack3(Session *pSession, char *pPacket);
};