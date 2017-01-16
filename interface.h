#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"
#include "parser.h"

class Interface {
private:
	Battlefield *Field;
	std::string EnemyField;

public:
	Interface(void);
	~Interface(void);

	void Start(void);
	bool GameControl(std::string mode);
};

#endif // INTERFACE_H
