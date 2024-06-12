#pragma once

void makePacketSCCreateMyCharacter(PacketSCCreateMyCharacter *packet, INT id, CHAR dir, USHORT x, USHORT y, BYTE hp);
void makePacketSCCreateOtherCharacter(PacketSCCreateOtherCharacter *packet, INT id, CHAR dir, USHORT x, USHORT y, CHAR hp);
void makePacketSCDeleteCharacter(PacketSCDeleteCharacter *packet, INT id);
void makePacketSCMoveStart(PacketSCMoveStart *packet, INT id, CHAR dir, USHORT x, USHORT y);
void makePacketSCMoveStop(PacketSCMoveStop *packet, INT id, CHAR dir, USHORT x, USHORT y);
void makePacketSCAttack1(PacketSCAttack1 *packet, INT id, CHAR dir, USHORT x, USHORT y);
void makePacketSCAttack2(PacketSCAttack2 *packet, INT id, CHAR dir, USHORT x, USHORT y);
void makePacketSCAttack3(PacketSCAttack3 *packet, INT id, CHAR dir, USHORT x, USHORT y);
void makePacketSCDamage(PacketSCDamage *packet, INT attacker, INT attacked, CHAR damage);


void makePacketCSMoveStart(PacketCSMoveStart *packet, CHAR dir, USHORT x, USHORT y);
void makePacketCSMoveStop(PacketCSMoveStop *packet, CHAR dir, USHORT x, USHORT y);
void makePacketCSAttack1(PacketCSAttack1 *packet, CHAR dir, USHORT x, USHORT y);
void makePacketCSAttack2(PacketCSAttack2 *packet, CHAR dir, USHORT x, USHORT y);
void makePacketCSAttack3(PacketCSAttack3 *packet, CHAR dir, USHORT x, USHORT y);