#include "parser.h"

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"
#include "converter.h"
#include "package.h"
#include <cstdio>
#include <string.h>

using namespace std;


string GetParameter(string str, size_t num) {
	str = RemoveSpecial(str);
	size_t cnt = 0;
	int i;
	bool special = false;
	for (i = 0; cnt < num && i < str.size(); i++) {
		if (special) {
			special = false;
			continue;
		}
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			cnt++;
		}
		if (str[i] == '\\') {
		   special = true;
		}
	}

	special = false;
	string res = "";
	for (; cnt == num && i < str.size(); i++) {
		if (special) {
			res += str[i];
			special = false;
			continue;
		}

		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			break;
		}
		if (str[i] == '\\') {
		   special = true;
		   continue;
		}
		res += str[i];
	}
	return res;
}

size_t ColToUNum(string str) {
	return (size_t) (str[0] - 'a');
}
size_t RowToUNum(string str) {
	return StringToUNum(str) - 1;
}

//========================

bool InstallParser(string cmd, Battlefield *field, bool *game_continue) {
	//set 4 vertical a 1
	//delete a 1
	*game_continue = true;
	cmd = StringToLower(cmd);
	if (cmd == "exit" || cmd.size() == 0) {
		if (cmd.size() == 0) {
			cout << "exit" << endl;
		}
		cout << "До свидания!" << endl;
		*game_continue = false;
		return false;
	}
	string action = GetParameter(cmd, 0);
	if (action == "set") {
		string length_str = GetParameter(cmd, 1);
		if (!IsStringNum(length_str)) {
			cout << "Некорректный параметр `размер`" << endl;
			return true;
		}
		size_t length = StringToUNum(length_str);
		string orientation_str = GetParameter(cmd, 2);
		char orientation = 0;
		if (orientation_str == "vertical") {
			orientation = VERTICAL;
		} else if (orientation_str == "horisontal") {
			orientation = HORISONTAL;
		} else {
			cout << "Ориентация задана некорректно" << endl;
			return true;
		}

		size_t col = ColToUNum(GetParameter(cmd, 3));
		size_t row = RowToUNum(GetParameter(cmd, 4));
		Position pos(col, row);

		Warship *ship = new Warship;
		if (!ship->Configure(pos, orientation, length, field, true)) {
			delete ship;
			return true;
		}
		cout << "Корабль успешно установлен" << endl;
		field->Print();
		field->Ships.Print();
		return true;
	}
	if (action == "delete") {
		size_t col = ColToUNum(GetParameter(cmd, 1));
		size_t row = RowToUNum(GetParameter(cmd, 2));
		Position pos(col, row);
		if (pos.X >= BATTLEFIELD_SIZE || pos.Y >= BATTLEFIELD_SIZE) {
			cout << "Некорректная позиция" << endl;
			return true;
		}
		if (field->Map[pos.X][pos.Y] == NULL) {
			cout << "На позиции " << pos << " корабль отсутствует" << endl;
			return true;
		}
		delete field->Map[pos.X][pos.Y];
		cout << "Корабль успешно удалён" << endl;
		field->Print();
		field->Ships.Print();
		return true;
	}
	if (action == "random") {
		field->Clear();
		field->RandomFill();
		field->Print();
		return true;
	}
	if (action == "clear") {
		field->Clear();
		field->Print();
		return true;
	}
	if (action == "help") {
		cout << "Команды настройки игровой карты:" << endl;
		cout << "set <длина> <VERTICAL/HORISONTAL> <буква> <число> - установить корабль" << endl;
		cout << "Пример: \n`>>> set 3 VERTICAL a 3`"<< endl;
		cout << "delete <буква> <число> - удалить корабль (возможен ввод любой клетки корабля)" << endl;
		cout << "Пример: \n`>>> delete a 4`" << endl;
		cout << "random - установить случайную позицию" << endl;
		cout << "clear - очистить поле" << endl;
		cout << "start - начать игру" << endl;
		//cout << "show - отобразить поле" << endl;
		cout << "exit - выход из игры" << endl;
		cout << "help - список доступных в данный момент команд" << endl;
		return true;
	}
	if (action == "start") {
		if (field->Ships.TotalFree() > 0) {
			cout << "Вы должы поставить на поле все имеющиеся корабли" << endl;
			return true;
		}
		return false;
	}
	
	cout << "Неизвестная команда `" << action << "`" << endl;
	return true;
}

bool BattleParser(string cmd, Battlefield *field, Package *package, bool *game_continue) {
	*game_continue = true;
	cmd = StringToLower(cmd);
	package->IsAnswer = true;
	string action = GetParameter(cmd, 0);
	if (action == "shot") {
		string message = "";
		size_t col = ColToUNum(GetParameter(cmd, 1));
		size_t row = RowToUNum(GetParameter(cmd, 2));
		Position pos(col, row);
		if (pos.X >= BATTLEFIELD_SIZE || pos.Y >= BATTLEFIELD_SIZE) {
			message = "Некорректная позиция";
			cout << message << endl;
			StringToBas(message, package->Message);
			return true;
		}
		if (field->Fire(pos)) {
			message = "Попадание!";
			cout << message << endl;
			StringToBas(message, package->Message);
			package->IsHit = true;
		} else {
			message = "Промах!";
			cout << message << endl;
			StringToBas(message, package->Message);
		}
		string map_new = field->GetMap(true);
		package->IsMap = true;
		StringToBas(map_new, package->Map);
		if (field->IsGameOver()) {
			message = "Игра окончена!";
			cout << message << endl;
			StringToBas(message, package->Message);
			package->IsGameOver = true;
			return false;
		}
		return true;
	}

	if (action == "help") {
		cout << "Команды геймплея:" << endl;
		cout << "shot <буква> <число> - произвести выстрел" << endl;
		cout << "Пример: \n`>>> shot a 3`"<< endl;
		cout << "exit - выход из игры" << endl;
		cout << "help - список доступных в данный момент команд" << endl;
		return true;
	}
	if (action == "exit" || action == "") {
		string message = "Соперник вышел из игры";		
		cout << message << endl;
		StringToBas(message, package->Message);
		return false;
	}
	string message = "Неизвестная команда `" + action + "`\n";
	StringToBas(message, package->Message);
	return true;
}
