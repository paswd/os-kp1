#ifndef BATTLEOBJECTS_H
#define BATTLEOBJECTS_H

#include "geometry.h"
#include <string>

const size_t BATTLEFIELD_SIZE = 10;
const size_t SHIP_MAX_LENGTH = 4;
const char VERTICAL = 'v';
const char HORISONTAL = 'h';
const char FIRST_LETTER = 'A';

//Graphics
const char GRAPHICS_SHIP = 'S';
const char GRAPHICS_SHIP_DEAD = 'X';
const char GRAPHICS_SPACE = '.';
const char GRAPHICS_SPACE_DEAD = '-';

class Battlefield;

class FreeShips {
private:
	size_t Arr[SHIP_MAX_LENGTH];

public:
	FreeShips(void);
	~FreeShips(void);

	bool UseShip(size_t length);
	bool IsUsable(size_t length);
	void FreeShip(size_t length);
	void Print(void);
};

class Warship {
private:
	Position Pos;
	char Orientation;
	size_t Length;
	Battlefield *Field;
	size_t Lifes;

	void PrintErrorMessage(size_t error);
	bool IsCorrectData(Position pos, char orientation, size_t length, Battlefield *field,  bool show_errors);

public:
	Warship(void);
	~Warship(void);
	bool Configure(Position pos, char orientation, size_t length, Battlefield *field, bool show_errors);

	Position GetPosition(void);
	char GetOrientation(void);
	size_t GetLength(void);
	void Hit(void);
	bool IsDead(void);
	void SetBorder(void);
};

class Battlefield {
private:
	bool Visibility;

	void PrintSymPublic(Position pos);
	void PrintSymPrivate(Position pos);

public:
	Warship *Map[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];
	bool Shots[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];

	FreeShips Ships;
	Battlefield(bool visibility);
	~Battlefield(void);

	void Print(void);
	bool Fire(Position pos);
	void RandomFill(void);
	void Clear(void);
};

#endif // BATTLEOBJECTS_H
