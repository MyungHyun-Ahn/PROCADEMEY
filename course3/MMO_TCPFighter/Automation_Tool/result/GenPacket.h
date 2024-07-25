#pragma once
class GenPacket
{
public:
	static void makePacketSCCreateMyCharacter(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	static void makePacketSCCreateOtherCharacter(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	static void makePacketSCDeleteCharacter(Session *pSession, INT id);

	static void makePacketSCMoveStart(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCMoveStop(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack1(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack2(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCAttack3(Session *pSession, INT id, CHAR direction, USHORT x, USHORT y);

	static void makePacketSCDamage(Session *pSession, INT attackId, INT damageId, CHAR damageHp);

	static void makePacketSCSync(Session *pSession, INT id, USHORT x, USHORT y);

	static void makePacketSCEcho(Session *pSession, DWORD time);
};
