#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include "Coordinate.hpp"
#include <vector>

#define PLAYER_SYMBOL 'P'
#define ENEMY_SYMBOL 'X'
#define BULLET_SYMBOL 'o'
#define ENEMY_BULLET_SYMBOL 'u'

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
	virtual std::vector<Object *> update(int ch, int t) = 0;
};

#endif