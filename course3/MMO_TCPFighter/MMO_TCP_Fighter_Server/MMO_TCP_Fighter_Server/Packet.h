#pragma once
#pragma once

// 패킷 코드
enum class PACKET_CODE
{
	PACKET_SC_CREATE_MY_CHARACTER = 0,
	PACKET_SC_CREATE_OTHER_CHARACTER = 1,
	PACKET_SC_DELETE_CHARACTER = 2,
	PACKET_CS_MOVE_START = 10,
	PACKET_SC_MOVE_START = 11,
	PACKET_CS_MOVE_STOP = 12,
	PACKET_SC_MOVE_STOP = 13,
	PACKET_CS_ATTACK1 = 20,
	PACKET_SC_ATTACK1 = 21,
	PACKET_CS_ATTACK2 = 22,
	PACKET_SC_ATTACK2 = 23,
	PACKET_CS_ATTACK3 = 24,
	PACKET_SC_ATTACK3 = 25,
	PACKET_SC_DAMAGE = 30,

	// 사용안함
	PACKET_CS_SYNC = 250,
	PACKET_SC_SYNC = 251
};

#pragma pack(push, 1)
struct PacketHeader
{
	BYTE		byCode;
	BYTE		bySize;
	BYTE		byType;
};

struct PacketSCCreateMyCharacter : public PacketHeader
{
	INT			id;
	CHAR		direction;
	USHORT		x;
	USHORT		y;
	BYTE		hp;
};

struct PacketSCCreateOtherCharacter : public PacketHeader
{
	INT			id;
	CHAR		direction;
	USHORT		x;
	USHORT		y;
	CHAR		hp;
};

struct PacketSCDeleteCharacter : public PacketHeader
{
	INT			id;
};

struct PacketCSMoveStart : public PacketHeader
{
	CHAR		moveDir;
	USHORT		x;
	USHORT		y;
};

struct PacketSCMoveStart : public PacketHeader
{
	INT			id;
	CHAR		direction;
	USHORT		x;
	USHORT		y;
};

struct PacketCSMoveStop : public PacketHeader
{
	CHAR		viewDir;
	USHORT		x;
	USHORT		y;
};

struct PacketSCMoveStop : public PacketHeader
{
	INT			id;
	CHAR		direction;
	USHORT		x;
	USHORT		y;
};

struct PacketCSAttack1 : public PacketHeader
{
	CHAR		direction;
	USHORT		x;
	USHORT		y;
};

struct PacketSCAttack1 : public PacketHeader
{
	INT			id;
	CHAR		viewDir;
	USHORT		x;
	USHORT		y;
};

struct PacketCSAttack2 : public PacketHeader
{
	CHAR		direction;
	USHORT		x;
	USHORT		y;
};

struct PacketSCAttack2 : public PacketHeader
{
	INT			id;
	CHAR		viewDir;
	USHORT		x;
	USHORT		y;
};

struct PacketCSAttack3 : public PacketHeader
{
	CHAR		direction;
	USHORT		x;
	USHORT		y;
};

struct PacketSCAttack3 : public PacketHeader
{
	INT			id;
	CHAR		viewDir;
	USHORT		x;
	USHORT		y;
};

struct PacketSCDamage : public PacketHeader
{
	INT			attackerId;
	INT			attackedId;
	CHAR		hpToReduce;
};

#pragma pack(pop)