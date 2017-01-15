#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"

using namespace std;

int main(void) {
	setlocale(LC_ALL, "russian");
	Battlefield field(true);
	field.Print();
	Warship *ship = new Warship();
	Position pos1(1, 1);
	ship->Configure(pos1, VERTICAL, 2, &field);
	field.Fire(pos1);
	Position pos2(3, 4);
	field.Fire(pos2);
	//ship->Configure(pos2, HORISONTAL, 2, &field);
	field.Print();
	
	return 0;
}
