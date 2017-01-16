#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"
#include "converter.h"
#include "package.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

std::string GetParameter(std::string str, size_t num);

bool InstallParser(std::string cmd, Battlefield *field, bool *game_continue);
bool BattleParser(std::string cmd, Battlefield *field, Package *package, bool *game_continue);

#endif // PARSER_H
