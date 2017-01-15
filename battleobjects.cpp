#include "battleobjects.h"
#include <iostream>
#include "geometry.h"
#include <string>

using namespace std;

/*
=========
FreeShips
=========
*/

FreeShips::FreeShips(void) {
	for (size_t i = 0; i < SHIP_MAX_LENGTH; i++) {
		this->Arr[i] = SHIP_MAX_LENGTH - i;
	}
}
FreeShips::~FreeShips(void) {

}
bool FreeShips::UseShip(size_t length) {
	if (length > SHIP_MAX_LENGTH || length == 0) {
		return false;
	}
	size_t len_pos = length - 1;
	if (this->Arr[len_pos] == 0) {
		return false;
	}
	this->Arr[len_pos]--;
	return true;
}
void FreeShips::FreeShip(size_t length) {
	if (length > SHIP_MAX_LENGTH || length == 0) {
		return;
	}
	this->Arr[length - 1]++; 
}
void FreeShips::Print(void) {
	for (size_t i = 0; i < SHIP_MAX_LENGTH; i++) {
		cout << i + 1 << "-палубный: " << this->Arr[i] << " шт." << endl;
	}
}

/*
=======
Warship
=======
*/

Warship::Warship(void) {
	this->Orientation = 0;
	this->Length = 0;
}
Warship::~Warship(void) {
	if (this->Field == NULL) {
		return;
	}
	if (this->Orientation == VERTICAL) {
		for (size_t i = this->Pos.Y; i < this->Pos.Y + this->Length; i++) {
			this->Field->Map[this->Pos.X][i] = NULL;
		}
	} else {
		for (size_t i = this->Pos.X; i < this->Pos.X + this->Length; i++) {
			this->Field->Map[i][this->Pos.Y] = NULL;
		}
	}
}
void Warship::PrintErrorMessage(size_t error) {
	switch (error) {
		case 1:
			cout << "Ошибка: неправильная позиция" << endl;
			break;
		case 2:
			cout << "Ошибка: неправильная ориентация" << endl;
			break;
		default:
			cout << "Ошибка конфигурации корабля" << endl;
	}
}
bool Warship::IsCorrectData(Position pos, char orientation, size_t length, Battlefield *field) {
	if (orientation != VERTICAL && orientation != HORISONTAL) {
		this->PrintErrorMessage(2);
		return false;
	}
	if (orientation == VERTICAL) {		
		for (size_t i = pos.Y - 1; i <= pos.Y + length; i++) {
			if (i >= BATTLEFIELD_SIZE && i != pos.Y - 1 && i != pos.Y + length) {
				return false;
			} else if (i < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X][i] != NULL) {
					return false;
				}
			}
		
			if (i < BATTLEFIELD_SIZE && pos.X - 1 < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X - 1][i] != NULL) {
					this->PrintErrorMessage(1);
					return false;
				}
			}
			if (i < BATTLEFIELD_SIZE && pos.X + 1 < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X + 1][i] != NULL) {
					this->PrintErrorMessage(1);
					return false;
				}
			}
		}
	} else {
		for (size_t i = pos.X - 1; i <= pos.X + length; i++) {
			if (i >= BATTLEFIELD_SIZE && i != pos.X - 1 && i != pos.X + length) {
				return false;
			} else if (i < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y] != NULL) {
					return false;
				}
			}
		
			if (i < BATTLEFIELD_SIZE && pos.Y - 1 < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y - 1] != NULL) {
					this->PrintErrorMessage(1);
					return false;
				}
			}
			if (i < BATTLEFIELD_SIZE && pos.Y + 1 < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y + 1] != NULL) {
					this->PrintErrorMessage(1);
					return false;
				}
			}
		}
	}

	return true;
}
bool Warship::Configure(Position pos, char orientation, size_t length, Battlefield *field) {
	if (!IsCorrectData(pos, orientation, length, field)) {
		return false;
	}
	if (!field->Ships.UseShip(length)) {
		return false;
	}
	this->Pos = pos;
	this->Orientation = orientation;
	this->Length = length;
	this->Field = field;

	if (this->Orientation == VERTICAL) {
		for (size_t i = this->Pos.Y; i < this->Pos.Y + length; i++) {
			this->Field->Map[this->Pos.X][i] = this;
		}
	} else {
		for (size_t i = this->Pos.X; i < this->Pos.X + length; i++) {
			this->Field->Map[i][this->Pos.Y] = this;
		}
	}
	return true;
}
Position Warship::GetPosition(void) {
	return this->Pos;
}
char Warship::GetOrientation(void) {
	return this->Orientation;
}
size_t Warship::GetLength(void) {
	return this->Length;
}


/*
===========
Battlefield
===========
*/

Battlefield::Battlefield(bool visibility) {
	for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j < BATTLEFIELD_SIZE; j++) {
			this->Map[i][j] = NULL;
			this->Shots[i][j] = false;
		}
	}
	this->Visibility = visibility;
}
Battlefield::~Battlefield(void) {
	for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j < BATTLEFIELD_SIZE; j++) {
			if (this->Map[i][j] != NULL) {
				delete this->Map[i][j];
			}
		}
	}
}
void Battlefield::PrintSymPublic(Position pos) {
	if (this->Map[pos.X][pos.Y] != NULL) {
		if (this->Shots[pos.X][pos.Y]) {
			cout << 'X';
		} else {
			cout << '*';
		}
	} else {
		if (this->Shots[pos.X][pos.Y]) {
			cout << 'o';
		} else {
			cout << '.';
		}
	}
}
void Battlefield::PrintSymPrivate(Position pos) {
	if (this->Shots[pos.X][pos.Y]) {
		if (this->Map[pos.X][pos.Y] != NULL) {
			cout << 'X';
		} else {
			cout << 'o';
		}
	} else {
		cout << '.';
	}
}
void Battlefield::Print(void) {
	for (size_t i = 0; i <= BATTLEFIELD_SIZE; i++) {
		if (i == 0) {
			cout << ' ';
		} else {
			cout << (char) (i + FIRST_LETTER - 1);
		}
		cout << ' ';
	}
	cout << endl;
	for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j <= BATTLEFIELD_SIZE; j++) {
			if (j == 0) {
				cout << i + 1;
			} else {
				Position pos(j - 1, i);
				if (this->Visibility) {
					PrintSymPublic(pos);
				} else {
					PrintSymPrivate(pos);
				}
			}
			cout << ' ';
		}
		cout << endl;
	}
}
bool Battlefield::Fire(Position pos) {
	if (this->Shots[pos.X][pos.Y]) {
		return false;
	}
	this->Shots[pos.X][pos.Y] = true;
	return this->Map[pos.X][pos.Y] != NULL;
}
