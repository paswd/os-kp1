#include "battleobjects.h"
#include <iostream>
#include "geometry.h"
#include "converter.h"
#include <string>

using namespace std;

/*
=========
FreeShips
=========
*/

FreeShips::FreeShips(void) {
	this->MaxPoints = 0;
	for (size_t i = 0; i < SHIP_MAX_LENGTH; i++) {
		this->Arr[i] = SHIP_MAX_LENGTH - i;
		this->MaxPoints += (i + 1) * (SHIP_MAX_LENGTH - i);
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
bool FreeShips::IsUsable(size_t length) {
	if (length > SHIP_MAX_LENGTH || length == 0) {
		return false;
	}
	return this->Arr[length - 1] > 0;
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
size_t FreeShips::GetMaxPoints(void) {
	return this->MaxPoints;
}
size_t FreeShips::TotalFree(void) {
	size_t res = 0;
	for (size_t i = 0; i < SHIP_MAX_LENGTH; i++) {
		res += this->Arr[i];
	}
	return res;
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
	this->Field->Ships.FreeShip(this->Length);
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
			break;
	}
}
bool Warship::IsCorrectData(Position pos, char orientation, size_t length, Battlefield *field, bool show_errors) {
	if (orientation != VERTICAL && orientation != HORISONTAL) {
		if (show_errors) {
			this->PrintErrorMessage(2);
		}
		return false;
	}
	if (orientation == VERTICAL) {		
		for (size_t i = min(pos.Y - 1, pos.Y); i <= pos.Y + length; i++) {
			if (i >= BATTLEFIELD_SIZE && i != pos.Y - 1 && i != pos.Y + length) {
				if (show_errors) {
					this->PrintErrorMessage(1);
				}
				return false;
			} else if (i < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X][i] != NULL) {
					if (show_errors) {
						this->PrintErrorMessage(1);
					}
					return false;
				}
			}
		
			if (i < BATTLEFIELD_SIZE && pos.X - 1 < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X - 1][i] != NULL) {
					if (show_errors) {
						this->PrintErrorMessage(1);
					}
					return false;
				}
			}
			if (i < BATTLEFIELD_SIZE && pos.X + 1 < BATTLEFIELD_SIZE) {
				if (field->Map[pos.X + 1][i] != NULL) {
					if (show_errors) {
						this->PrintErrorMessage(1);
					}
					return false;
				}
			}
		}
	} else {
		for (size_t i = min(pos.X - 1, pos.X); i <= pos.X + length; i++) {
			if (i >= BATTLEFIELD_SIZE && i != pos.X - 1 && i != pos.X + length) {
				if (show_errors) {
					this->PrintErrorMessage(1);
				}
				return false;
			} else if (i < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y] != NULL) {
					if (show_errors) {
						this->PrintErrorMessage(1);
					}
					return false;
				}
			}
		
			if (i < BATTLEFIELD_SIZE && pos.Y - 1 < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y - 1] != NULL) {
					if (show_errors) {
						this->PrintErrorMessage(1);
					}
					return false;
				}
			}
			if (i < BATTLEFIELD_SIZE && pos.Y + 1 < BATTLEFIELD_SIZE) {
				if (field->Map[i][pos.Y + 1] != NULL) {
					if (show_errors) {
						this->PrintErrorMessage(1);
					}
					return false;
				}
			}
		}
	}

	return true;
}
bool Warship::Configure(Position pos, char orientation, size_t length, Battlefield *field,  bool show_errors) {
	if (!IsCorrectData(pos, orientation, length, field, show_errors)) {
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

Battlefield::Battlefield(void) {
	this->Points = 0;
	for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j < BATTLEFIELD_SIZE; j++) {
			this->Map[i][j] = NULL;
			this->Shots[i][j] = false;
		}
	}
	//this->Visibility = visibility;
}
Battlefield::~Battlefield(void) {
	this->Clear();
}
char Battlefield::GetSymPublic(Position pos) {
	if (this->Map[pos.X][pos.Y] != NULL) {
		if (this->Shots[pos.X][pos.Y]) {
			return GRAPHICS_SHIP_DEAD;
		} else {
			return GRAPHICS_SHIP;
		}
	} else {
		if (this->Shots[pos.X][pos.Y]) {
			return GRAPHICS_SPACE_DEAD;
		} else {
			return GRAPHICS_SPACE;
		}
	}
}
char Battlefield::GetSymPrivate(Position pos) {
	if (this->Shots[pos.X][pos.Y]) {
		if (this->Map[pos.X][pos.Y] != NULL) {
			return GRAPHICS_SHIP_DEAD;
		} else {
			return GRAPHICS_SPACE_DEAD;
		}
	} else {
		return GRAPHICS_SPACE;
	}
}
string Battlefield::GetMap(bool is_private) {
	string res = "";
	for (size_t i = 0; i <= BATTLEFIELD_SIZE; i++) {
		if (i == 0) {
			res += "   ";
		} else {
			res += (char) (i + FIRST_LETTER - 1);
		}
		res += ' ';
	}
	res += '\n';
	for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j <= BATTLEFIELD_SIZE; j++) {
			if (j == 0) {
				//cout << i + 1;
				size_t num = i + 1;
				
				if (num < 10) {
					res += ' ';
				}
				res += UNumToString(num) + ' ';
			} else {
				Position pos(j - 1, i);
				if (!is_private) {
					res += GetSymPublic(pos);
				} else {
					res += GetSymPrivate(pos);
				}
				
			}
			res += ' ';
		}
		res += '\n';
	}
	res += '\n';
	return res;
}
void Battlefield::Print(void) {
	cout << this->GetMap(false);
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
		this->Points++;
	}

	return res;
}
void Battlefield::RandomFill(void) {
	char orientation_variants[2];
	orientation_variants[0] = VERTICAL;
	orientation_variants[1] = HORISONTAL;
	Warship *ship = NULL;
	for (size_t i = SHIP_MAX_LENGTH; i > 0; i--) {
		while (this->Ships.IsUsable(i)) {
			//this->Ships.Print();
			Position pos(rand() % (BATTLEFIELD_SIZE - i + 1), rand() % (BATTLEFIELD_SIZE - i + 1));
			//cout << i << endl;
			//cout << pos << endl;
			char orientation = orientation_variants[rand() % 2];
			//cout << orientation << endl;
			//Warship *ship = new Warship;
			if (ship == NULL) {
				ship = new Warship;
			}
			if (!ship->Configure(pos, orientation, i, this, false)) {
				//delete ship;
				//cout << "False" << endl;
				//this->Ships.FreeShip(i);
			} else {
				//cout << "True" << endl;
				ship = NULL;
			}
		}
	}
}
void Battlefield::Clear(void) {
	this->Points = 0;
	//cout << "Clear::True" << endl;
	for (size_t i = 0; i < BATTLEFIELD_SIZE; i++) {
		for (size_t j = 0; j < BATTLEFIELD_SIZE; j++) {
			if (this->Map[j][i] != NULL) {
				delete this->Map[j][i];
			}
		}
	}
}
bool Battlefield::IsGameOver(void) {
	return this->Points >= this->Ships.GetMaxPoints();
}
