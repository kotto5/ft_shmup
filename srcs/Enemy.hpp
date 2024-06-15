#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Object.hpp"
#include "EnemyBullet.hpp"

class Enemy : public Object
{
public:
	Enemy(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol, int frequency = 8) : Object(x, y, t0, speed, symbol, frequency) {}
	std::vector<Object *> update(int ch, int t) {
		(void)ch;
		(void)t;
		std::vector<Object *> objects;
		if (t % 10 == 5)
			objects.push_back(new EnemyBullet(x - 1 - t / frequency, y, t, [](int t) { return Coordinate(-t, 0); }, ENEMY_BULLET_SYMBOL));
		return objects;
	}
};

#endif
