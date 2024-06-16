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
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <ctime>
#include <stdlib.h>

#define UNBEATABLE false
#define FLAME_RATE 10
#define SPAWN_RATE 30
#define SPAWN_PER_TICK 15

int	main(void);

void center_print(const char *str, int y) {
  int w = getmaxx(stdscr);
  int x = (w - strlen(str)) / 2;
  mvprintw(y, x, str);
}

int	game_over() {
	clear();
  int h = getmaxy(stdscr);
  center_print("Game Over", h / 2);
  center_print("If you want to quit press 'q'.", h / 2 + 3);
  center_print("If you want to play again press 'n'.", h / 2 + 4);
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
  Coordinate player_c(1, 1);
  for (size_t i = 0; i < objects.size(); i++) {
    Coordinate c = objects[i]->get_coordinate(t);
    if (objects[i]->get_symbol() == PLAYER_SYMBOL) {
      player_c = c;
    }
    char synbol = objects[i]->get_symbol();
    char tmp[2] = {synbol, '\0'};
    mvprintw(c.y, c.x - t, tmp); // no camera move
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
  mvprintw(h - 1, 0, "Score: %d\tTotal Objects: %d\tTick: %zu", score, objects.size(), t);
  mvprintw(h - 1, w - 1 - 15, "Player: %d, %d", player_c.x, player_c.y);
  refresh();
  return 0;
}

std::tuple<std::vector<Object *>, int> collision(std::vector<Object *> objects, size_t t) {
  int gained_score = 0;

  for (size_t i = 0; i < objects.size(); i++) {
	for (size_t j = i + 1; j < objects.size(); j++) {
		if (objects[i]->get_coordinate(t) == objects[j]->get_coordinate(t)) {
			if (!UNBEATABLE && objects[i]->symbol == PLAYER_SYMBOL && objects[j]->symbol == ENEMY_SYMBOL) {
				game_over();
			}
			else if (objects[i]->symbol == ENEMY_SYMBOL && objects[j]->symbol == BULLET_SYMBOL) {
        		gained_score += 100;
				objects.erase(objects.begin() + i);
				objects.erase(objects.begin() + j - 1);
				i--;
				j = -2;
			}
			else if (!UNBEATABLE && objects[i]->symbol == PLAYER_SYMBOL && objects[j]->symbol == ENEMY_BULLET_SYMBOL) {
				game_over();
			}
		}
	}
  }
  return make_tuple(objects, gained_score);
}

/// @brief objectsを引数にとり、get_coordinateの情報を基に、不要なインスタンスを削除する
/// @param objects 削除判定前のobjects
/// @param t 
/// @return 不要なインスタンスが削除された状態のobjectsを返す
std::vector<Object *> delete_outside_objects(std::vector<Object *> objects, size_t t) {
  int  width;
  width = getmaxx(stdscr);
  int margin = 10;
  for (ssize_t i = objects.size() - 1; i >= 0; i--) {
    Coordinate c = objects[i]->get_coordinate(t);
    if (c.x < (int)t || c.x >= width + (int)t + margin) {
      objects.erase(objects.begin() + i);
    }
  }
  return objects;
}

int64_t datetime_millisec() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

char  get_input() {
  int ch = getch();
  while (getch() != ERR) {
    continue;
  }
  return ch;
}

std::vector<Object *> spawn(size_t t) {
  std::vector<Object *> objects;
  int  width, height;
  getmaxyx(stdscr, height, width);
  if (t % SPAWN_PER_TICK == 0 && rand() % 100 <= SPAWN_RATE){
    objects.push_back(new Enemy(t + width - 2, rand() % height - 1, t, [](int t) {return Coordinate(-t, 0); }, 'X'));
  }
  return objects;
}

size_t get_tick() {
  static size_t tick = 0;
  tick ++;
  return tick;
}

int init(std::vector<Object *> &objects) {
  initscr();
  noecho(); // キーが入力されても表示しない
  curs_set(0);// カーソルを非表示
  timeout(1);
  srand(time(0));
  objects.push_back(new Player(30, 5, 0, [](int t) { return Coordinate(t, 0); }, PLAYER_SYMBOL));
  return 0;
}

int add_list_to_list(std::vector<Object *> &objects, std::vector<Object *> new_objects) {
  for (size_t i = 0; i < new_objects.size(); i++) {
    objects.push_back(new_objects[i]);
  }
  return 0;
}

std::vector<Object *> update_status_and_produce_objects(std::vector<Object *> objects, int ch, size_t t) {
  for (size_t i = 0; i < objects.size(); i++) {
    std::vector<Object *> tmp = objects[i]->change_status_and_produce_objects(ch, t);
    for (size_t j = 0; j < tmp.size(); j++) {
      objects.push_back(tmp[j]);
    }
  }
  return objects;
}

int main(void) {
  std::vector<Object *> objects;
  init(objects);
  int score = 0;
  while (1) {
    int ch = get_input();
    if (ch == 'q') {
      break;
    }
    size_t frame_tick = get_tick();

    objects = update_status_and_produce_objects(objects, ch, frame_tick);
    add_list_to_list(objects, spawn(frame_tick));

    auto [new_objs, new_score] = collision(objects, frame_tick);
    objects = new_objs;
    score += new_score;
    objects = delete_outside_objects(objects, frame_tick);

    display(objects, score, frame_tick);
    usleep(1000 * 1000 / FLAME_RATE);
  }
}
