#include <iostream>
#include <string>
#include <time.h>
#include "geometry.h"
#include "battleobjects.h"
#include "interface.h"

using namespace std;

int main(void) {
	setlocale(LC_ALL, "russian");
	srand(time(NULL));

	Interface interface;
	interface.MapInstallation();

	//Battlefield field(true);
	/*field.Print();
	Warship *ship = new Warship;
	Position pos1(0, 0);
	ship->Configure(pos1, VERTICAL, 2, &field);
	field.Fire(pos1);
	Position pos2(3, 4);
	field.Fire(pos2);
	ship = new Warship;
	ship->Configure(pos2, HORISONTAL, 4, &field);
	Position pos3(0, 6);
	ship = new Warship;
	ship->Configure(pos3, HORISONTAL, 3, &field);
	Position pos4(0, 1);
	field.Fire(pos4);
	Position pos5(0, 0);
	field.Fire(pos5);*/
	//field.RandomFill();
	/*Warship *ship = new Warship;
	Position pos1(5, 1);
	ship->Configure(pos1, VERTICAL, 4, &field);*/
	
	//field.Print();
	
	return 0;
}
