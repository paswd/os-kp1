#ifndef BATTLEOBJECTS_H
#define BATTLEOBJECTS_H

#include "geometry.h"
#include <string>

const size_t BATTLEFIELD_SIZE = 9;
const size_t SHIP_MAX_LENGTH = 4;
const char VERTICAL = 'v';
const char HORISONTAL = 'h';
const char FIRST_LETTER = 'A';

class Battlefield;

class FreeShips {
private:
	size_t Arr[SHIP_MAX_LENGTH];

public:
	FreeShips(void);
	~FreeShips(void);

	bool UseShip(size_t length);
	void FreeShip(size_t length);
	void Print(void);
};

class Warship {
private:
	Position Pos;
	char Orientation; //'v' (vertical) or 'h' (horisontal)
	size_t Length;
	Battlefield *Field;

	void PrintErrorMessage(size_t error);
	bool IsCorrectData(Position pos, char orientation, size_t length, Battlefield *field);

public:
	Warship(void);
	//Warship(Position pos, char orientation, size_t length);
	//Warship(Position pos, char orientation, size_t length, Battlefield *field);
	~Warship(void);
	//~Warship(Battlefield *field);
	bool Configure(Position pos, char orientation, size_t length, Battlefield *field);

	Position GetPosition(void);
	char GetOrientation(void);
	size_t GetLength(void);
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
};

#endif // BATTLEOBJECTS_H
