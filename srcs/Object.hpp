#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include "Coordinate.hpp"

struct Object
{
	int x;
	int y;
	std::function<Coordinate(int)> speed;
	Object(int x, int y, std::function<Coordinate(int)> speed) {
		this->x = x;
		this->y = y;
		this->speed = speed;
	}
	Coordinate get_coordinate(int t) {
		Coordinate s = speed(t);
		return Coordinate(x + s.x, y + s.y);
	}
};

#endif