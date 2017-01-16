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

void Interface::Start(void) {
	while (true) {
		if (!this->GameControl("install")) {
			break;
		}
		if (!this->GameControl("battle")) {
			break;
		}
	}
}

bool Interface::GameControl(string mode) {
	string cmd = "";
	this->Field->Print();
	if (mode == "install") {
		this->Field->Clear();
		cout << "Для получения текущего списка команд введите `help`" << endl;
	}
	bool game_continue;
	while (true) {
		cout << ">>> ";
		getline(cin, cmd);
		//this->Field->Clear();
		if (mode == "install") {
			//this->Field->Clear();
			if (!InstallParser(cmd, this->Field, &game_continue)) {
				break;
			}
		}
		if (mode == "battle") {
			if (!BattleParser(cmd, this->Field, &game_continue)) {
				break;
			}
		}
	}
	return game_continue;
}



