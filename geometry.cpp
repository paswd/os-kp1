#include <iostream>
#include <cstdlib>
#include <cmath>
#include "geometry.h"

using namespace std;

Position::Position(void) {
	this->X = 0;
	this->Y = 0;
}
Position::Position(size_t x, size_t y) {
	this->X = x;
	this->Y = y;
}
Position::~Position(void) {
	
}

void Position::Input(std::istream &is) {
	is >> this->X;
	is >> this->Y;
}

void Position::Print(void) {
	cout << "(" << this->X << ":" << this->Y << ")";
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
	os << "(" << pos.X << ":" << pos.Y << ")";
	return os;
}
std::istream& operator>>(std::istream& is, Position& pos) {
	is >> pos.X;
	is >> pos.Y;
	return is;
}
