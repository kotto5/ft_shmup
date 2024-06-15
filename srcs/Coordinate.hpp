#ifndef COORDINATE_HPP
#define COORDINATE_HPP

struct Coordinate {
    public:
    int x;
    int y;
    Coordinate(int x, int y) {
    this->x = x;
    this->y = y;
    }
	bool operator==(const Coordinate &rhs) const {
		return x == rhs.x && y == rhs.y;
	}
};

#endif
