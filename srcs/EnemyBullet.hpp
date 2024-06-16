#ifndef ENEMY_BULLET_HPP
#define ENEMY_BULLET_HPP

#include "Object.hpp"

class EnemyBullet : public Object
{
public:
	EnemyBullet(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol, int frequency = 1) : Object(x, y, t0, speed, symbol, frequency) {}
	std::vector<Object *> change_status_and_produce_objects(int ch, int t) {
		(void)ch;
		(void)t;
		std::vector<Object *> objects;
		return objects;
	}
};

#endif
