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
	if (this->Field != NULL) {
		delete this->Field;
	}
}

void Interface::MainConsoleEngine(void) {
	while (true) {
		if (!this->MapInstallation()) {
			break;
		}
	}
}

bool Interface::MapInstallation(void) {
	string cmd = "";
	this->Field->Print();
	cout << "Для получения списка команд введите `help`" << endl;
	bool game_continue;
	do {
		cout << ">>> ";
		getline(cin, cmd);
		this->Field->Clear();
	} while (InstallParser(cmd, this->Field, &game_continue));
	return game_continue;
}


