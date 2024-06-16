#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Object.hpp"

class Obstacle : public Object
{
public:
    Obstacle(int x, int y, int t0, std::function<Coordinate(int)> speed, char symbol) : Object(x, y, t0, speed, symbol) {}
    std::vector<Object *> change_status_produce_objects(int ch, int t) {
        (void)ch;
        (void)t;
        std::vector<Object *> objects;
        return objects;
    }
};

#endif
