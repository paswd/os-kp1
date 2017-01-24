#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"
#include "parser.h"
#include "package.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>


class Interface {
private:
	Battlefield *Field;
	std::string EnemyField;
	bool IsServer;

	//Networking
	int ToServer;
	int ToClient;
	std::string File1;
	std::string File2;
public:
	Interface(void);
	~Interface(void);

	void Start(void);
	bool InstallControl(void);
	bool GameControl(void);
	void SelectNetworkRole(void);
	std::string GetEmptyMap(void);
	void PrintMaps(void);
};

#endif // INTERFACE_H
