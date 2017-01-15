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

public:
	Interface(void);
	~Interface(void);

	void MainConsoleEngine(void);
	bool MapInstallation(void);
};

#endif // INTERFACE_H
