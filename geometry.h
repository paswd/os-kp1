#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cstdlib>

//Класс для хранения и обработки координат
class Position {
public:
	size_t X;
	size_t Y;

	Position(void);
	Position(size_t x, size_t y);
	~Position(void);

	void Input(std::istream &is);
	void Print(void);

	friend std::ostream& operator<<(std::ostream& os, const Position& pos);
	friend std::istream& operator>>(std::istream& is, Position& obj);
};


#endif /* GEOMETRY_H */
