#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"

using namespace std;

int main(void) {
	setlocale(LC_ALL, "russian");
	Battlefield field(true);
	field.Print();
	Warship *ship = new Warship;
	Position pos1(1, 1);
	ship->Configure(pos1, VERTICAL, 2, &field);
	field.Fire(pos1);
	Position pos2(3, 4);
	field.Fire(pos2);
	ship = new Warship;
	ship->Configure(pos2, HORISONTAL, 4, &field);
	Position pos3(0, 6);
	ship = new Warship;
	ship->Configure(pos3, HORISONTAL, 3, &field);
	Position pos4(1, 2);
	field.Fire(pos4);
	field.Print();
	
	return 0;
}
