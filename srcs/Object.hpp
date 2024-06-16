#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include "Coordinate.hpp"
#include <vector>

#include<string>

#define PLAYER_SYMBOL 'P'
#define ENEMY_SYMBOL 'X'
#define BULLET_SYMBOL 'o'
#define ENEMY_BULLET_SYMBOL '*'
#define BLOCK_SYMBOL '#'

struct Object
{
	int x;
	int y;
	int t0;
	char symbol;
	std::string tag;

	size_t frequency;
	std::function<Coordinate(int)> speed;
	Object(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol, int frequency = 4, std::string tag = "") {
		this->x = x;
		this->y = y;
		this->t0 = t0;
		this->speed = speed;
		this->symbol = symbol;
		this->frequency = frequency;
		this->tag = tag;
	}
	std::string get_tag() {
		return tag;
	}
	char get_symbol() {
		return symbol;
	}
	Coordinate get_coordinate(int t) {
		Coordinate s = speed((t - t0)); // speed が2 * tの関数の時、1マス飛ばしで出現する弾丸になる。
		return Coordinate(x + s.x, y + s.y);
	}
	virtual std::vector<Object *> change_status_and_produce_objects(int ch, int t) = 0; // return new objects created by this object
};

#endif
