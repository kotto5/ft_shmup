#include <ncurses.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <functional>
#include "Coordinate.hpp"
#include "Object.hpp"

// #define PLAYER_SYMBOL "(^_^)"
// #define ENEMY_SYMBOL "(X_X)"

#define PLAYER_SYMBOL 'P'
#define ENEMY_SYMBOL 'X'
#define BULLET_SYMBOL 'o'

#define FLAME_RATE 100000

int	main(void);

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

void collision(std::vector<Object> objects) {
  // player vs enemy: player dies
  // enemy vs bullet(player): enemy dies
  // player vs bullet(enemy): player dies
  // player vs obstacle: dont't move | player dies
  // enemy vs obstacle: no effect
  Object player = objects[0];
  for (size_t i = 1; i < objects.size(); i++) {
    if (objects[i].symbol == ENEMY_SYMBOL) { // player vs bullet(enemy): player dies
      Object enemy = objects[i];
      if (player.x == enemy.x && player.y == enemy.y) {
        game_over();
      }
    }
    else if (objects[i].symbol == BULLET_SYMBOL) // enemy vs bullet(player): enemy dies | player vs bullet(enemy): player dies
    {
      Object bullet = objects[i];
      for (size_t i = 1; i < objects.size(); i++) {
        if (objects[i].symbol == ENEMY_SYMBOL) {
          if (bullet.x == objects[i].x && bullet.y == objects[i].y) {
            objects.erase(objects.begin() + i);
          }
        }
      }
    }


  }
}

bool collide(Coordinate player, std::vector<Coordinate> enemies) {
	for (size_t i = 0; i < enemies.size(); i++) {
		if (player.x == enemies[i].x && player.y == enemies[i].y) {
			return true;
		}
	}
	return false;
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
      break;
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
    collision(objects);
	// if (collide(player, enemies))
	// 	return game_over();
	// else
	display(objects, t);
	usleep(FLAME_RATE);
	t++;
  }
}
