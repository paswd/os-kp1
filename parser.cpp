#include "parser.h"

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"

using namespace std;

bool IsSeparator(char sym) {
	switch (sym) {
		case '\t':
		case '\n':
		case '\v':
		case '\f':
		case '\r':
		case ' ':
		case '\0':
			return true;
		default:
			return false;
	}
}
string StringReverse(std::string str) {
	string new_str = "";
	for (size_t i = str.size(); i > 0; i--) {
		new_str += str[i - 1];
	}
	return new_str;
}

char CharToLower(char sym) {
	int diff = 'a' - 'A';
	if (sym >= 'A' && sym <= 'Z') {
		sym += diff;
	}
	return (char) sym;
}

string StringToLower(string str) {
	for (size_t i = 0; i < str.size(); i++) {
		str[i] = CharToLower(str[i]);
	}
	return str;
}

string RemoveSpecial(string str) {
	string new_str = "";
	bool is_first_space = true;
	for (size_t i = 0; i < str.size(); i++) {
		if (!IsSeparator(str[i])) {
			is_first_space = true;
		}
		bool is_set_space = false;
		if (str[i] == ' ' && is_first_space) {
			is_set_space = true;
			is_first_space = false;
		}

		if (!IsSeparator(str[i]) || is_set_space) {
			new_str += str[i];
		}
	}
	return new_str;
}

bool IsCharNum(char sym) {
	return sym >= '0' && sym <= '9';
}
bool IsStringNum(string str) {
	for (size_t i = 0; i < str.size(); i++) {
		if (!IsCharNum(str[i])) {
			return false;
		}
	}
	return true;
}

char UNumToChar(size_t num) {
	return num + '0';
}
string UNumToString(size_t num) {
	string str = "";
	while (num > 0) {
		size_t md = num % 10;
		num /= 10;
		str += UNumToChar(md);
	}
	return StringReverse(str);
}
size_t CharToUNum(char sym) {
	if (IsCharNum(sym)) {
		return sym - '0';
	}
	return 0;
}
size_t StringToUNum(string str) {
	size_t res = 0;
	for (size_t i = 0; i < str.size(); i++) {
		res *= 10;
		res += CharToUNum(str[i]);
	}
	return res;
}

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

bool InstallParser(string cmd, Battlefield *field) {
	//set 4 vertical a 1
	//delete a 1
	cmd = StringToLower(cmd);
	if (cmd == "exit" || cmd.size() == 0) {
		if (cmd.size() == 0) {
			cout << "exit" << endl;
		}
		cout << "До свидания!" << endl;
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
		cout << "exit - выход из игры" << endl;
		cout << "help - список доступных в данный момент команд" << endl;
		return true;
	}
	if (action == "start") {
		cout << "Игра начинается... (в разработке)" << endl;
		return false;
	}
	
	cout << "Неизвестная команда `" << action << "`" << endl;
	return true;
}
