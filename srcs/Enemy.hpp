#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Object.hpp"
#include "EnemyBullet.hpp"

class Enemy : public Object
{
public:
	Enemy(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
	std::vector<Object *> change_status_and_produce_objects(int ch, int t) {
		(void)ch;
		std::vector<Object *> objects;
		if (t % 20 == 5) {
			Coordinate bullet_spawn = this->get_coordinate(t);
			objects.push_back(new EnemyBullet(bullet_spawn.x, bullet_spawn.y, t, [](int t) { return Coordinate(-t, 0); }, ENEMY_BULLET_SYMBOL));
		}
		return objects;
	}
};

#endif
