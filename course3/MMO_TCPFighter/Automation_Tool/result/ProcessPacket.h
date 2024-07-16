#pragma once
class GenPacket
{
public:
	void makePacketSCCreateMyCharacter(Session *session, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	void makePacketSCCreateOtherCharacter(Session *session, INT id, CHAR direction, USHORT x, USHORT y, BYTE hp);

	void makePacketSCDeleteCharacter(Session *session, INT id);

	void makePacketSCMoveStart(Session *session, INT id, CHAR direction, USHORT x, USHORT y);

	void makePacketSCMoveStop(Session *session, INT id, CHAR direction, USHORT x, USHORT y);

	void makePacketSCAttack1(Session *session, INT id, CHAR direction, USHORT x, USHORT y);

	void makePacketSCAttack2(Session *session, INT id, CHAR direction, USHORT x, USHORT y);

	void makePacketSCAttack3(Session *session, INT id, CHAR direction, USHORT x, USHORT y);

	void makePacketSCDamage(Session *session, INT attackId, INT damageId, 1 damageHp);

	void makePacketSCSync(Session *session, INT id, USHORT x, USHORT y);

	void makePacketSCEcho(Session *session, DWORD time);
};
