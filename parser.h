#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "geometry.h"
#include "battleobjects.h"

bool IsSeparator(char sym);
std::string StringReverse(std::string str);
char CharToLower(char sym);
std::string StringToLower(std::string str);
std::string RemoveSpecial(std::string str);
bool IsCharNum(char sym);
bool IsStringNum(std::string str);
char UNumToChar(size_t num);
std::string UNumToString(size_t num);
size_t CharToUNum(char sym);
size_t StringToUNum(std::string str);
std::string GetParameter(std::string str, size_t num);

bool InstallParser(std::string cmd, Battlefield *field, bool *game_continue);

#endif // PARSER_H
