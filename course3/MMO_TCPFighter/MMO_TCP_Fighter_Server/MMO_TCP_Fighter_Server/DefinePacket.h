#pragma once

#define dfPACKET_CODE 0x89

#pragma pack(push, 1)
struct PacketHeader
{
	BYTE		byCode;
	BYTE		bySize;
	BYTE		byType;
};
#pragma pack(pop)

enum class PACKET_CODE
{
	SCCreateMyCharacter = 0,
};