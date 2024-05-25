#pragma once

enum class PacketCode
{
	AllocID = 0,
	CreateStar = 1,
	DestroyStar = 2,
	MoveStar = 3,
};

struct PacketBase
{
	int Type;
	int ID;
	char dummy[8];
};

struct PacketAllocID
{
	int Type;
	int ID;
	char dummy[8];
};

struct PacketCreateStar
{
	int Type;
	int ID;
	int X;
	int Y;
};

struct PacketDestroyStar
{
	int Type;
	int ID;
	char dummy[8];
};

struct PacketMoveStar
{
	int Type;
	int ID;
	int X;
	int Y;
};