#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Object.hpp"
#include "Bullet.hpp"

class Player : public Object
{
public:
	Player(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
	std::vector<Object *> update(int ch, int t) {
		int width, height;
		getmaxyx(stdscr, height, width);
		// t の原則処理 frame_tick -> game_tick
		int left_bound = t + 2;
		int right_bound = left_bound + width - 1;

		std::vector<Object *> objects;
		if (ch == 'w') {
			this->y == 1 ? this->y = 1 : this->y--;	
		}
		else if (ch == 's') {
			this->y == height - 3 ? this->y = height - 3 : this->y++;
		}
		else if (ch == 'a') {
			this->x == left_bound ? this->x == left_bound : this->x--;
		}
		else if (ch == 'd') {
			this->x == right_bound  ? this->x = right_bound : this->x++;
		}
		else if (ch == ' ') {
			objects.push_back(new Bullet(x, y, t, [](int t) { return Coordinate(t, 0); }, BULLET_SYMBOL, 1));
		}
		// else {
		// 	this->x == left_bound + 1 ? left_bound + 2: this->x--;
		// }
		return objects;
	}
};

#endif
