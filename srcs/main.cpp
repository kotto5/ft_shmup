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

Coordinate update(Coordinate player, int ch) {
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
  }
  return player;
}

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

Coordinate get_coordinate(int t) {
	return Coordinate(5, 5 + 2 * t);
}

int	display(Coordinate player, std::vector<Coordinate> enemies) {
	clear();
	mvprintw(player.y, player.x, PLAYER_SYMBOL);
	for (size_t i = 0; i < enemies.size(); i++) {
		mvprintw(enemies[i].y, enemies[i].x, ENEMY_SYMBOL);
	}
	refresh();
	return 0;
}

int display(std::vector<Object> objects, size_t t) {
  clear();
  for (size_t i = 0; i < objects.size(); i++) {
	Coordinate c = objects[i].get_coordinate(t);
	mvprintw(c.y, c.x, PLAYER_SYMBOL);
  }
  refresh();
  return 0;
}

Coordinate no_speed(int t) {
  (void) t;
  return Coordinate(t, 0);
}

int main(void) {
  std::vector<Object> objects;
  objects.push_back(Object(10, 10, [](int t) { return Coordinate(t, 0); }));

//   Coordinate player(0, 0);
//   std::vector<Coordinate> enemies;
//   enemies.push_back(Coordinate(10, 10));
//   enemies.push_back(Coordinate(20, 20)); 
  initscr();
  size_t t = 0;
  while (1) {
    // int ch = getch();
    // if (ch == 'q') {
    //   break;
    // }

    // update(player, ch);
	// if (collide(player, enemies))
	// 	return game_over();
	// else
	display(objects, t);
	usleep(FLAME_RATE);
	t++;
  }
}
