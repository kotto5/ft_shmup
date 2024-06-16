#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <string>
#include "Object.hpp"

class Block : public Object
{
public:
	int width;
	int height;
	char texture;

	Block(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol, int width, int height, std::string tag = "block") : Object(x, y, t0, speed, symbol, tag) {
		this->width = width;
		this->height = height;
		this->texture = symbol;
	}

	std::vector<Object *> change_status_and_produce_objects(int ch, int t) {
		(void)ch;
		(void)t;
		std::vector<Object *> objects;
		return objects;
	}
};

#endif
