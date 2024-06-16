#ifndef BULLET_HPP
#define BULLET_HPP

#include <vector>
#include "Object.hpp"

class Bullet : public Object
{
public:
	Bullet(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
	std::vector<Object *> change_status_and_produce_objects(int ch, int t) {
		(void)ch;
		(void)t;
		std::vector<Object *> objects;
		return objects;
	}
};

#endif
