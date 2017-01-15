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
	//this->Print();
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
	this->Lifes = 0;
}
Warship::~Warship(void) {
	//cout << "Pos: " << this->Pos << endl;
	//cout << "Point1" << endl;
	if (this->Field == NULL) {
		return;
	}
	//cout << "Point2" << endl;
	if (this->Orientation == VERTICAL) {
		//cout << "Point3:1" << endl;
		for (size_t i = this->Pos.Y; i < this->Pos.Y + this->Length; i++) {
			this->Field->Map[this->Pos.X][i] = NULL;
		}
		//cout << "Point4:1" << endl;
	} else {
		//cout << "Point3:2" << endl;
		for (size_t i = this->Pos.X; i < this->Pos.X + this->Length; i++) {
			this->Field->Map[i][this->Pos.Y] = NULL;
		}
		//cout << "Point4:2" << endl;
	}
	//cout << "-----------------" << endl;
	/*for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j < BATTLEFIELD_SIZE; j++) {
			if (this->Field->Map[j][i] != NULL) {
				cout << 1;
			} else {
				cout << 0;
			}
			cout << ' ';
		}
		cout << endl;
	}*/
	//cout << "=================" << endl;
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
		for (size_t i = min(pos.Y - 1, pos.Y); i <= pos.Y + length; i++) {
			if (i >= BATTLEFIELD_SIZE && i != pos.Y - 1 && i != pos.Y + length) {
				this->PrintErrorMessage(1);
				return false;
			} else if (i < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X][i] != NULL) {
					this->PrintErrorMessage(1);
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
		//cout << pos << endl;
		for (size_t i = min(pos.X - 1, pos.X); i <= pos.X + length; i++) {
			//cout << "Correct: " << i << endl;
			if (i >= BATTLEFIELD_SIZE && i != pos.X - 1 && i != pos.X + length) {
				this->PrintErrorMessage(1);
				return false;
			} else if (i < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y] != NULL) {
					this->PrintErrorMessage(1);
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
	this->Lifes = this->Length;
	this->Field = field;

	if (this->Orientation == VERTICAL) {
		for (size_t i = this->Pos.Y; i < this->Pos.Y + length; i++) {
			//cout << "{" << this->Pos.X << ":" << i << "}" << endl;
			this->Field->Map[this->Pos.X][i] = this;
		}
	} else {
		for (size_t i = this->Pos.X; i < this->Pos.X + length; i++) {
			//cout << "{" << i << ":" << this->Pos.Y << "}" << endl;
			this->Field->Map[i][this->Pos.Y] = this;
		}
	}
	//cout << "-----" << endl;
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
void Warship::Hit(void) {
	if (this->Lifes > 0) {
		this->Lifes--;
	}
}
bool Warship::IsDead(void) {
	return this->Lifes == 0;
}
void Warship::SetBorder(void) {
	if (this->Orientation == VERTICAL) {
		for (size_t i = min(this->Pos.Y - 1, this->Pos.Y); i <= this->Pos.Y + this->Length; i++) {
			//this->Field->Map[this->Pos.X][i] = NULL;
			for (size_t j = min(this->Pos.X - 1, this->Pos.X); j <= this->Pos.X + 1; j++) {
				if (i < BATTLEFIELD_SIZE && j < BATTLEFIELD_SIZE) {
					if (this->Field->Map[j][i] == NULL) {
						this->Field->Shots[j][i] = true;
					}
				}
			}
		}
	} else {
		for (size_t i = min(this->Pos.X - 1, this->Pos.X); i <= this->Pos.X + this->Length; i++) {
			//this->Field->Map[i][this->Pos.Y] = NULL;
			for (size_t j = min(this->Pos.Y - 1, this->Pos.Y); j <= this->Pos.Y + 1; j++) {
				if (i < BATTLEFIELD_SIZE && j < BATTLEFIELD_SIZE) {
					if (this->Field->Map[i][j] == NULL) {
						this->Field->Shots[i][j] = true;
					}
				}
			}
		}
	}
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
			if (this->Map[j][i] != NULL) {
				//cout << "[" << j << ":" << i << "]" << endl;
				delete this->Map[j][i];
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
	cout << endl;
}
bool Battlefield::Fire(Position pos) {
	if (this->Shots[pos.X][pos.Y]) {
		return false;
	}
	this->Shots[pos.X][pos.Y] = true;
	bool res = this->Map[pos.X][pos.Y] != NULL;
	if (res) {
		this->Map[pos.X][pos.Y]->Hit();
		if (this->Map[pos.X][pos.Y]->IsDead()) {
			this->Map[pos.X][pos.Y]->SetBorder();
		}
	}

	return res;
}

