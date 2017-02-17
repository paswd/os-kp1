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

bool ContinueMessage(string str) {
	if (str == "Некорректная позиция") {
		return true;
	}
	if (str == "Попадание!") {
		return true;
	}
	if (str.find("Неизвестная команда") != string::npos) {
		return true;
	}
	if (str.find("Вы уже стреляли по этой клетке") != string::npos) {
		return true;
	}
	return false;
}
bool IsMapUpdated(string str) {
	if (str == "Попадание!") {
		return true;
	}
	return false;
}
bool IsGameOver(string str) {
	if (str == "Соперник вышел из игры") {
		return true;
	}
	return false;
}


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
		Package package;
		if (this->IsServer) {
			
			SetEmptyPackage(&package);
			package.Ready = true;
			read(ToServer, &package, sizeof(Package));\
			write(ToClient, &package, sizeof(Package));

		} else {
			SetEmptyPackage(&package);
			package.Ready = true;
			write(ToServer, &package, sizeof(Package));
			read(ToClient, &package, sizeof(Package));
		}
		cout << "Игра начинается..." << endl;
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
	string value_str;
	getline(cin, value_str);
	size_t value = StringToUNum(value_str);
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
		getline(cin, cmd);\
		if (!InstallParser(cmd, this->Field, &game_continue)) {
			break;
		}
		
	}
	return game_continue;
}
bool Interface::GameControl(void) {
	string cmd = "";
	bool game_continue = true;
	bool is_first = true;
	bool first = true;
	while (true) {
		Package package;
		SetEmptyPackage(&package);
		bool cont = true;
		bool is_fst = true;
		if (this->IsServer) {
			while (!package.Status && !package.IsGameOver && cont) {
				if (first) {
					first = false;
					this->PrintMaps();
				}
				if (is_fst) {
					is_fst = false;
				} else {
					string msg_tmp(package.Message);
					if (IsMapUpdated(msg_tmp)) {
						this->PrintMaps();
					}
				}
				read(ToServer, &package, sizeof(Package));
				if (package.Exit) {
					cout << "Соперник вышел из игры" << endl;
					//cout << "exit1" << endl;
					game_continue = false;
					return false;
				}
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
			while (!package.Status && !package.IsGameOver && cont) {
				if (is_first) {
					break;
				}
				if (is_fst) {
					is_fst = false;
				} else {
					string msg_tmp(package.Message);
					if (IsMapUpdated(msg_tmp)) {
						this->PrintMaps();
					}
				}
				read(ToClient, &package, sizeof(Package));
				if (package.Exit) {
					game_continue = false;
					cout << "Соперник вышел из игры" << endl;
					return false;
				}
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
		if (!cont && (!is_first || this->IsServer)) {
			return false;
		}
		bool is_continue = true;
		while (is_continue) {
			is_first = false;
			bool cntn = false;
			this->PrintMaps();
			SetEmptyPackage(&package);
			do {
				cntn = false;
				cout << ">>> ";
				getline(cin, cmd);
				cmd = StringToLower(cmd);
				if (cmd == "help") {
					cout << "Команды геймплея:" << endl;
					cout << "shot <буква> <число> - произвести выстрел" << endl;
					cout << "Пример: \n`>>> shot a 3`"<< endl;
					cout << "exit - выход из игры" << endl;
					cout << "help - список доступных в данный момент команд" << endl;
					cntn = true;
				} else if (cmd == "exit" || cmd.size() == 0) {
					string message = "";
					if (cmd.size() == 0) {
						message += "exit\n";
					}
					message += "До свидания!";
					cout << message << endl;
					StringToBas(message, package.Message);
					package.Exit = true;
					game_continue = false;
				}
			} while (cntn);
			
			SetEmptyPackage(&package);
			package.IsQuestion = true;
			StringToBas(cmd, package.Cmd);;
			if (this->IsServer) {
				write(ToClient, &package, sizeof(Package));
				if (!game_continue) {
					break;
				}
				read(ToServer, &package, sizeof(Package));
				if (package.IsMap) {
					string new_map(package.Map);
					this->EnemyField = new_map;
				}

				string tmp_msg(package.Message);
				cout << tmp_msg << endl;
				
				if (package.Exit) {
					return false;
				}
				SetEmptyPackage(&package);
				if (!ContinueMessage(tmp_msg)) {
					package.Status = true;
					is_continue = false;
				}
				write(ToClient, &package, sizeof(Package));
			} else {
				write(ToServer, &package, sizeof(Package));
				if (!game_continue) {
					break;
				}
				read(ToClient, &package, sizeof(Package));

				if (package.IsMap) {
					string new_map(package.Map);
					this->EnemyField = new_map;
				}

				string tmp_msg(package.Message);
				cout << tmp_msg << endl;

				if (package.Exit) {
					return false;
				}
				SetEmptyPackage(&package);
				if (!ContinueMessage(tmp_msg)) {
					package.Status = true;
					is_continue = false;
				}
				write(ToServer, &package, sizeof(Package));
			}
		}
		
		if (!game_continue) {
			return false;
		}
		this->PrintMaps();
	}
	return game_continue;
}
string Interface::GetEmptyMap(void) {
	Battlefield *field = new Battlefield;
	string res = field->GetMap(true);
	delete field;
	return res;
}
void Interface::PrintMaps(void) {
	cout << "Ваше поле:" << endl;
	this->Field->Print();
	cout << "Поле противника:" << endl;
	cout << this->EnemyField << endl;
}
