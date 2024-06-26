#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Object.hpp"
#include "Bullet.hpp"

class Player : public Object
{
public:
	Player(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
	std::vector<Object *> change_status_and_produce_objects(int ch, int t) {
		int width, height;
		getmaxyx(stdscr, height, width);
		int left_bound = t + 1;
		int right_bound = left_bound + width - 3;
		Coordinate coo = get_coordinate(t);
		if (coo.x == left_bound) { // border barriers
			this->x++;
			coo = get_coordinate(t);
		}
		std::vector<Object *> objects;
		if (ch == 'w') {
			this->y += coo.y - 1 <= 0 ? 0 : -1;
		}
		else if (ch == 's') {
			this->y += coo.y + 1 >= height - 1 ? 0 : 1;
		}
		else if (ch == 'a') {
			this->x += coo.x - 1 <= left_bound ? 0 : -1;
		}
		else if (ch == 'd') {
			this->x += coo.x + 1 >= right_bound ? 0 : 1;
		}
		else if (ch == ' ') {
			Coordinate bullet_spawn = this->get_coordinate(t);
			objects.push_back(new Bullet(bullet_spawn.x, bullet_spawn.y, t, [](int t) { return Coordinate(t * 3, 0); }, BULLET_SYMBOL));
			objects.push_back(new Bullet(bullet_spawn.x, bullet_spawn.y, t, [](int t) { return Coordinate(t * 3, 0.3 * t); }, BULLET_SYMBOL));
			objects.push_back(new Bullet(bullet_spawn.x, bullet_spawn.y, t, [](int t) { return Coordinate(t * 3, -0.3 * t); }, BULLET_SYMBOL));
		}
		return objects;
	}
};

#endif
