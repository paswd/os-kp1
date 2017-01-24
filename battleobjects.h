#ifndef BATTLEOBJECTS_H
#define BATTLEOBJECTS_H

#include "geometry.h"
#include "converter.h"
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
const char GRAPHICS_SPACE_DEAD = 'o';

const size_t MAP_SIZE = 512;

class Battlefield;

class FreeShips {
private:
	size_t Arr[SHIP_MAX_LENGTH];
	size_t MaxPoints;

public:
	FreeShips(void);
	~FreeShips(void);

	bool UseShip(size_t length);
	bool IsUsable(size_t length);
	void FreeShip(size_t length);
	void Print(void);
	size_t GetMaxPoints(void);
	size_t TotalFree(void);
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
	size_t Points;

	char GetSymPublic(Position pos);
	char GetSymPrivate(Position pos);

public:
	Warship *Map[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];
	bool Shots[BATTLEFIELD_SIZE][BATTLEFIELD_SIZE];

	FreeShips Ships;
	Battlefield(void);
	~Battlefield(void);

	std::string GetMap(bool is_private);
	void Print(void);
	bool Fire(Position pos);
	void RandomFill(void);
	void Clear(void);
	bool IsGameOver(void);
};

#endif // BATTLEOBJECTS_H
