#include <ncurses.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <functional>
#include "Coordinate.hpp"
#include "Object.hpp"
#include <tuple>
#include "Player.hpp"
#include "Enemy.hpp"

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
			exit(0);
			return 0;
		}
		else if (ch == 'n')
			return main();
	}
}

int display(std::vector<Object *> objects, int score, size_t t) {
  clear();
  for (size_t i = 0; i < objects.size(); i++) {
	Coordinate c = objects[i]->get_coordinate(t);
	char synbol = objects[i]->get_symbol();
	char tmp[2] = {synbol, '\0'};
	mvprintw(c.y, c.x, tmp);
  }
  int  x, y, w, h;
  x = 0;
  getmaxyx(stdscr, h, w);
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      if (y == 0 || y == h - 2) {
        mvprintw(y, x, "-");
      }
      if ((x == 0 || x == w - 1) && (y != 0 && y < h - 2)) {
        mvprintw(y, x, "|");
      }
    }
    x = 0;
    
  }
  mvprintw(h - 1, 0, "Score: %d", score);
  refresh();
  return 0;
}

std::tuple<std::vector<Object *>, int> collision(std::vector<Object *> objects, size_t t) {
  // player vs enemy: player dies
  // enemy vs bullet(player): enemy dies
  // player vs bullet(enemy): player dies
  // player vs obstacle: dont't move | player dies
  // enemy vs obstacle: no effect
  int gained_score = 0;

  for (size_t i = 0; i < objects.size(); i++) {
	for (size_t j = i + 1; j < objects.size(); j++) {
		if (objects[i]->get_coordinate(t) == objects[j]->get_coordinate(t)) {
			if (objects[i]->symbol == PLAYER_SYMBOL && objects[j]->symbol == ENEMY_SYMBOL) {
				game_over();
			}
			else if (objects[i]->symbol == ENEMY_SYMBOL && objects[j]->symbol == BULLET_SYMBOL) {
        		gained_score += 100;
				objects.erase(objects.begin() + i);
				objects.erase(objects.begin() + j - 1);
				i--;
				j = -2;
			}
			else if (objects[i]->symbol == PLAYER_SYMBOL && objects[j]->symbol == ENEMY_BULLET_SYMBOL) {
				game_over();
			}
		}
	}
  }
  return make_tuple(objects, gained_score);
}

int main(void) {
  std::vector<Object *> objects;
  size_t t = 0;
  objects.push_back(new Player(10, 10, t, [](int t) { (void)t; return Coordinate(0, 0); }, 'P'));
  //enemy
  objects.push_back(new Enemy(20, 10, t, [](int t) { (void)t; return Coordinate(0, 0); }, 'X'));
  int score = 0;
  initscr();
  timeout(1);
  while (1) {
    int ch = getch();
    if (ch == 'q') {
      break;
    }
	std::vector<Object *> new_objects;
	for (size_t i = 0; i < objects.size(); i++) {
		std::vector<Object *> tmp = objects[i]->update(ch, t);
		for (size_t j = 0; j < tmp.size(); j++) {
			new_objects.push_back(tmp[j]);
		}
	}
	for (size_t i = 0; i < new_objects.size(); i++) {
		objects.push_back(new_objects[i]);
	}
    auto [new_objs, new_score] = collision(objects, t);
    objects = new_objs;
    score += new_score;
    display(objects, score, t);
    usleep(FLAME_RATE);
    t++;
  }
}
