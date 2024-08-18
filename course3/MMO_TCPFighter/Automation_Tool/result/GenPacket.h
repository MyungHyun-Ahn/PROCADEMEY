#pragma once
class GenPacket
{
public:
	static void makePacketSCCreateMyCharacter(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	static void makePacketSCCreateOtherCharacter(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	static void makePacketSCDeleteCharacter(BOOL bIsBroadcast, Session *pSession, INT id);

	static void makePacketSCMoveStart(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCMoveStop(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack1(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack2(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack3(BOOL bIsBroadcast, Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCDamage(BOOL bIsBroadcast, Session *pSession, INT attackId, INT damageId, CHAR damageHp);

	static void makePacketSCSync(BOOL bIsBroadcast, Session *pSession, INT id, USHORT x, USHORT y);

	static void makePacketSCEcho(BOOL bIsBroadcast, Session *pSession, DWORD time);
};
