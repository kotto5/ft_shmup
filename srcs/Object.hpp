#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include "Coordinate.hpp"

struct Object
{
	int x;
	int y;
	int t0;
	char symbol;
	std::function<Coordinate(int)> speed;
	Object(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) {
		this->x = x;
		this->y = y;
		this->t0 = t0;
		this->speed = speed;
		this->symbol = symbol;
	}
	char get_symbol() {
		return symbol;
	}
	Coordinate get_coordinate(int t) {
		Coordinate s = speed(t - t0);
		return Coordinate(x + s.x, y + s.y);
	}
};

#endif