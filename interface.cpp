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
	return false;
}
bool IsMapUpdated(string str) {
	if (str == "Попадание!") {
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
			//Package got;
			read(ToServer, &package, sizeof(Package));
			/*if (package.Ready) {
				cout << "True" << endl;
			} else {
				cout << "False" << endl;
			}*/
			write(ToClient, &package, sizeof(Package));

		} else {
			SetEmptyPackage(&package);
			package.Ready = true;
			//cout << package.Num << endl;
			//Package got;
			write(ToServer, &package, sizeof(Package));
			read(ToClient, &package, sizeof(Package));
		}
		cout << "Игра начинается..." << endl;
		/*if (this->IsServer) {
			read(ToServer, &package, sizeof(Package));
		}*/
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
	//this->Field->Print();
	//cout << this->EnemyField << endl;
	bool game_continue = true;
	bool is_first = true;
	while (true) {
		Package package;
		SetEmptyPackage(&package);
		bool cont;
		cout << "Point" << endl;
		bool is_fst = true;
		if (this->IsServer) {
			while (!package.Status && !package.IsGameOver) {
				if (is_first) {
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
				cout << "PointIn1" << endl;
				read(ToServer, &package, sizeof(Package));
				if (package.Exit) {
					cout << "Соперник прервал игру" << endl;
					game_continue = false;
					return false;
				}
				cout << "PointIn2" << endl;
				if (package.Status) {
					break;
				}
				cout << "PointIn3" << endl;
				if (package.IsQuestion) {
					//cout << "Cmd::Bas: `" << package.Cmd << "`" << endl;
					string cmd(package.Cmd);
					//cout << "Cmd::Str: `" << cmd << "`" << endl;
					SetEmptyPackage(&package);
					cont = BattleParser(cmd, this->Field, &package, &game_continue);
					//cout << package.Message << endl;
					write(ToClient, &package, sizeof(Package));
				}
			}
		} else {
			//cout << "Point" << endl;
			while (!package.Status && !package.IsGameOver) {
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
				cout << "PointIn2" << endl;
				read(ToClient, &package, sizeof(Package));
				if (package.Exit) {
					game_continue = false;
					cout << "Соперник прервал игру" << endl;
					return false;
				}
				if (package.Status) {
					break;
				}
				if (package.IsQuestion) {
					string cmd(package.Cmd);
					SetEmptyPackage(&package);
					cont = BattleParser(cmd, this->Field, &package, &game_continue);
					//cout << package.Message << endl;
					write(ToServer, &package, sizeof(Package));
				}
			}
		}
		if (!cont && (!is_first || this->IsServer)) {
			return false;
		}
		bool is_continue = true;
		while (is_continue) {
			//is_continue = false;
			is_first = false;
			bool cntn = false;
			this->PrintMaps();
			//cout << "Msg: " << package.Message << endl;
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
					//cout << "restart - сдаться и начать игру сначала" << endl;
					//cout << "show - отобразить поле" << endl;
					cout << "exit - выход из игры" << endl;
					cout << "help - список доступных в данный момент команд" << endl;
					cntn = true;
				} else if (cmd == "exit" || cmd.size() == 0) {
					string message = "";
					if (cmd.size() == 0) {
						message += "exit\n";
					}
					//cout << "До свидания!" << endl;
					message += "До свидания!";
					cout << message << endl;
					//strcpy(strdup(message.c_str()), package.Message);
					StringToBas(message, package.Message);
					package.Exit = true;
					game_continue = false;
				}
			} while (cntn);
			
			SetEmptyPackage(&package);
			package.IsQuestion = true;
			cout << "Cmd::Str: `" << cmd << "`" << endl;
			//strcpy(strdup(cmd.c_str()), package.Cmd);
			StringToBas(cmd, package.Cmd);
			cout << "Cmd::Bas: `" << package.Cmd << "`" << endl;
			if (this->IsServer) {
				write(ToClient, &package, sizeof(Package));
				if (!game_continue) {
					break;
				}
				cout << "PointIn3" << endl;
				read(ToServer, &package, sizeof(Package));
				if (package.IsMap) {
					string new_map(package.Map);
					this->EnemyField = new_map;
				}
				//this->Field->Print();
				//cout << this->EnemyField << endl;

				cout << package.Message << endl;
				string tmp_msg(package.Message);
				
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
				cout << "PointIn4" << endl;
				read(ToClient, &package, sizeof(Package));

				if (package.IsMap) {
					string new_map(package.Map);
					this->EnemyField = new_map;
				}
				//this->Field->Print();
				//cout << this->EnemyField << endl;

				cout << package.Message << endl;
				string tmp_msg(package.Message);

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
		this->PrintMaps();
		cout << "Msg: " << package.Message << endl;
		if (!game_continue) {
			return false;
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
void Interface::PrintMaps(void) {
	cout << "Ваше поле:" << endl;
	this->Field->Print();
	cout << "Поле противника:" << endl;
	cout << this->EnemyField << endl;
}
