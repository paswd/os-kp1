#include "interface.h"

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"
#include "parser.h"

using namespace std;

Interface::Interface(void) {
	this->Field = new Battlefield(true);
}
Interface::~Interface(void) {
	delete this->Field;
}

void Interface::MapInstallation(void) {
	string cmd = "";
	this->Field->Print();
	cout << "Для получения списка команд введите `help`" << endl;
	do {
		cout << ">>> ";
		getline(cin, cmd);
	} while (InstallParser(cmd, this->Field));
}


