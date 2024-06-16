#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Object.hpp"
#include "Bullet.hpp"

class Player : public Object
{
public:
	Player(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol, int frequency = 8) : Object(x, y, t0, speed, symbol, frequency) {}
	std::vector<Object *> change_status_and_produce_objects(int ch, int t) {
		int width, height;
		getmaxyx(stdscr, height, width);
		int left_bound = t + 1;
		int right_bound = left_bound + width;
		Coordinate coo = get_coordinate(t);
		if (coo.x == left_bound) { // border barriers
			this->x++;
		}
		std::vector<Object *> objects;
		if (ch == 'w') {
			coo.y == 1 ? this->y = 1 : this->y--;	
		}
		else if (ch == 's') {
			coo.y == height - 3 ? this->y = height - 3 : this->y++;
		}
		else if (ch == 'a') {
			coo.x == left_bound ? this->x = left_bound : this->x--;
		}
		else if (ch == 'd') {
			this->x = (coo.x == right_bound ? right_bound - 1 : this->x + 1);
		}
		else if (ch == ' ') {
			Coordinate bullet_spawn = this->get_coordinate(t);
			objects.push_back(new Bullet(bullet_spawn.x, bullet_spawn.y, t, [](int t) { return Coordinate(t * 3, 0); }, BULLET_SYMBOL, 1));
		}
		return objects;
	}
};

#endif
