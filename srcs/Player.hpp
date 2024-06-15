#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "Object.hpp"
#include "Ballet.hpp"

class Player : public Object
{
public:
	Player(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
	std::vector<Object *> update(int ch, int t) {
		int width, height;
		getmaxyx(stdscr, height, width);
		std::vector<Object *> objects;
		if (ch == 'w') {
			this->y == 1 ? this->y = 1 : this->y--;	
		}
		else if (ch == 's') {
			this->y == height - 3 ? this->y = height - 3 : this->y++;
		}
		else if (ch == 'a') {
			this->x == 1 ? this->x = 1 : this->x--;
		}
		else if (ch == 'd') {
			this->x == width - 2 ? this->x = width - 2 : this->x++;
		}
		else if (ch == ' ') {
			objects.push_back(new Ballet(x, y, t, [](int t) { return Coordinate(t, 0); }, BULLET_SYMBOL));
		}
		return objects;
	}
};

#endif
