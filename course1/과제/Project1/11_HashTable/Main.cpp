#include <stdio.h>
#include "List.h"
#include "HashTable.h"

using namespace std;

struct Pos
{
	int x;
	int y;

	bool operator==(const Pos &other)
	{
		if (x == other.x && y == other.y)
			return true;

		return false;
	}

	Pos &operator=(const Pos &other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}
};

struct Player
{
	int attack;
	int defense;

	bool operator==(const Player &other)
	{
		if (attack == other.attack && defense == other.defense)
			return true;

		return false;
	}
};

int main()
{

	HashTable<Player, Pos> hash;

	{
		Player p1{ 100, 200 };
		Pos pos1{ 1, 1 };
		hash[p1] = pos1;
		hash.Insert({ 111, 111 }, { 11, 11 });
		hash.Insert({ 222, 222 }, { 22, 22 });
		hash.Insert({ 222, 222 }, { 22, 22 });
		hash.Delete(p1);
	}

	return 0;
}