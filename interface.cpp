#include "interface.h"

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"
#include "parser.h"
#include "package.h"
#include <string.h>
#include <cstdio>

using namespace std;


Interface::Interface(void) {
	this->Field = new Battlefield();
	this->File1 = "/tmp/.paswd-sea-battle-way1";
	this->File2 = "/tmp/.paswd-sea-battle-way2";
}
Interface::~Interface(void) {
	if (this->Field != NULL) {
		delete this->Field;
	}
	close(this->ToServer);
	close(this->ToClient);

	if (this->IsServer) {
		unlink(this->File1.c_str());
		unlink(this->File2.c_str());
	}
}

void Interface::Start(void) {
	while (true) {
		this->SelectNetworkRole();
		if (!this->InstallControl()) {
			break;
		}
		this->EnemyField = this->GetEmptyMap();
		cout << "Ожидание другого игрока..." << endl;
		if (this->IsServer) {
			Package package;
			SetEmptyPackage(&package);
			package.Ready = true;
			Package got;
			read(ToServer, &got, sizeof(Package));
			write(ToClient, &package, sizeof(Package));

		} else {
			Package package;
			SetEmptyPackage(&package);
			package.Ready = true;
			//Package got;
			write(ToServer, &package, sizeof(Package));
			//read(ToClient, &got, sizeof(Package));
		}
		if (!this->GameControl()) {
			break;
		}
	}
}

void Interface::SelectNetworkRole(void) {
	cout << "Выберите один из следующих пунктов:" << endl;
	cout << "(1) Создать игру (по умолчанию)" << endl;
	cout << "(2) Присоединиться к игре" << endl;
	cout << ">>> ";
	size_t value;
	cin >> value;
	if (value != 2) {
		this->IsServer = true;
		mkfifo(this->File1.c_str(), 0666);
		mkfifo(this->File2.c_str(), 0666);

		this->ToServer = open(this->File1.c_str(), O_RDONLY);
		this->ToClient = open(this->File2.c_str(), O_WRONLY);
	} else {
		this->IsServer = false;
		this->ToServer = open(this->File1.c_str(), O_WRONLY);
		this->ToClient = open(this->File2.c_str(), O_RDONLY);
	}

}
bool Interface::InstallControl(void) {
	string cmd = "";
	this->Field->Print();
	this->Field->Clear();
	cout << "Для получения текущего списка команд введите `help`" << endl;

	bool game_continue;
	while (true) {
		cout << ">>> ";
		getline(cin, cmd);
		//this->Field->Clear();
		//this->Field->Clear();
		if (!InstallParser(cmd, this->Field, &game_continue)) {
			break;
		}
		
	}
	return game_continue;
}
bool Interface::GameControl(void) {
	string cmd = "";
	this->Field->Print();
	bool game_continue;
	while (true) {
		Package package;
		SetEmptyPackage(&package);
		bool cont;
		if (this->IsServer) {
			while (!package.Status) {
				read(ToServer, &package, sizeof(Package));
				if (package.Status) {
					break;
				}
				if (package.IsQuestion) {
					string cmd(package.Cmd);
					SetEmptyPackage(&package);
					cont = BattleParser(cmd, this->Field, &package, &game_continue);
					write(ToClient, &package, sizeof(Package));
				}
			}
		} else {
			while (!package.Status) {
				read(ToClient, &package, sizeof(Package));
				if (package.Status) {
					break;
				}
				if (package.IsQuestion) {
					string cmd(package.Cmd);
					SetEmptyPackage(&package);
					cont = BattleParser(cmd, this->Field, &package, &game_continue);
					write(ToServer, &package, sizeof(Package));
				}
			}
		}
		if (!cont) {
			return false;
		}
		cout << ">>> ";
		getline(cin, cmd);
		SetEmptyPackage(&package);
		package.IsQuestion = true;
		strcpy(strdup(cmd.c_str()), package.Cmd);
		if (this->IsServer) {
			write(ToClient, &package, sizeof(Package));
			read(ToServer, &package, sizeof(Package));
			if (package.IsMap) {
				string new_map(package.Map);
				this->EnemyField = new_map;
			}
			this->Field->Print();
			cout << this->EnemyField << endl;

			cout << package.Message << endl;
			
			if (package.Exit) {
				return false;
			}
			SetEmptyPackage(&package);
			package.Status = true;
			write(ToClient, &package, sizeof(Package));
		} else {
			write(ToServer, &package, sizeof(Package));
			read(ToClient, &package, sizeof(Package));
			if (package.IsMap) {
				string new_map(package.Map);
				this->EnemyField = new_map;
			}
			this->Field->Print();
			cout << this->EnemyField << endl;

			cout << package.Message << endl;
			
			if (package.Exit) {
				return false;
			}
			SetEmptyPackage(&package);
			package.Status = true;
			write(ToServer, &package, sizeof(Package));
		}
		/*if (!BattleParser(cmd, this->Field, &game_continue)) {
			break;
		}*/
	}
	return game_continue;
}
string Interface::GetEmptyMap(void) {
	Battlefield *field = new Battlefield;
	string res = field->GetMap(true);
	delete field;
	return res;
}



