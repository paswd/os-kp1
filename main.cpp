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
	interface.MainConsoleEngine();
	
	return 0;
}
