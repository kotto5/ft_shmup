#ifndef ENEMY_BULLET_HPP
#define ENEMY_BULLET_HPP

#include "Object.hpp"

class EnemyBullet : public Object
{
public:
	EnemyBullet(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
	std::vector<Object *> update(int ch, int t) {
		(void)ch;
		(void)t;
		std::vector<Object *> objects;
		return objects;
	}
};

#endif
