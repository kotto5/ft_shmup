#include <ncurses.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <functional>
#include "Coordinate.hpp"
#include "Object.hpp"

#define PLAYER_SYMBOL "(^_^)"
#define ENEMY_SYMBOL "(X_X)"
#define FLAME_RATE 100000

int	main(void);

bool collide(Coordinate player, std::vector<Coordinate> enemies) {
	for (size_t i = 0; i < enemies.size(); i++) {
		if (player.x == enemies[i].x && player.y == enemies[i].y) {
			return true;
		}
	}
	return false;
}

int	game_over() {
	clear();
	mvprintw(10, 10, "Game Over\n If you want to quit press 'q'. If you want to play again press 'n'.");
	refresh();
	while (1) {
		int ch = getch();
		if (ch == 'q') {
			endwin();
			return 0;
		}
		else if (ch == 'n')
			return main();
	}
}

int display(std::vector<Object> objects, size_t t) {
  clear();
  for (size_t i = 0; i < objects.size(); i++) {
	Coordinate c = objects[i].get_coordinate(t);
	char synbol = objects[i].get_symbol();
	char tmp[2] = {synbol, '\0'};
	mvprintw(c.y, c.x, tmp);
  }
  refresh();
  return 0;
}

std::vector<Object> update(std::vector<Object> objects, int ch, int t) {
  Object player = objects[0];
  switch (ch) {
    case 'w':
      player.y--;
      break;
    case 's':
      player.y++;
      break;
    case 'a':
      player.x--;
      break;
    case 'd':
      player.x++;
      break;
	case ' ':
	  objects.push_back(Object(player.x, player.y, t, [](int t) { (void)t; return Coordinate(t, 0); }, 'o'));
  }
  objects[0] = player;
  return objects;
}

int main(void) {
  std::vector<Object> objects;
  size_t t = 0;
  objects.push_back(Object(10, 10, t, [](int t) { (void)t; return Coordinate(0, 0); }, 'P'));
  //enemy
  objects.push_back(Object(20, 10, t, [](int t) { (void)t; return Coordinate(0, 0); }, 'X'));

  initscr();
  timeout(1);
  while (1) {
    int ch = getch();
    if (ch == 'q') {
      break;
    }
    objects = update(objects, ch, t);
	// if (collide(player, enemies))
	// 	return game_over();
	// else
	display(objects, t);
	usleep(FLAME_RATE);
	t++;
  }
}
