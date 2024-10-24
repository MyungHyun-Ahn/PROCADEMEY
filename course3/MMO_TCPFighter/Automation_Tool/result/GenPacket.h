#pragma once
class GenPacket
{
public:
	static void makePacketSCCreateMyCharacter(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	static void makePacketSCCreateOtherCharacter(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	static void makePacketSCDeleteCharacter(BOOL bIsBroadcast, CSession *pSession, INT id);

	static void makePacketSCMoveStart(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCMoveStop(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack1(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack2(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack3(BOOL bIsBroadcast, CSession *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCDamage(BOOL bIsBroadcast, CSession *pSession, INT attackId, INT damageId, CHAR damageHp);

	static void makePacketSCSync(BOOL bIsBroadcast, CSession *pSession, INT id, USHORT x, USHORT y);

	static void makePacketSCEcho(BOOL bIsBroadcast, CSession *pSession, DWORD time);
};
