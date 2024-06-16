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
#include "Block.hpp"
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <ctime>
#include <stdlib.h>

#define UNBEATABLE false
#define FLAME_RATE 10
#define SPAWN_RATE 30
#define SPAWN_PER_TICK 15

// int	main(void);

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
  // center_print("If you want to play again press 'n'.", h / 2 + 4);
	refresh();
	while (1) {
		int ch = getch();
		if (ch == 'q') {
			endwin();
			exit(0);
			return 0;
		}
		// else if (ch == 'n')
		// 	return main();
	}
}

int display(std::vector<Object *> objects, int score, size_t t) {
  clear();
  for (size_t i = 0; i < objects.size(); i++) {
    char synbol = objects[i]->get_symbol();
    if (objects[i]->get_tag() == "block") {
      Block *block = dynamic_cast<Block *>(objects[i]);
      Coordinate c = block->get_coordinate(t);
      for (int w = 0; w < block->width; w++) {
        for (int h = 0; h < block->height; h++) {
          char tmp[2] = {synbol, '\0'};
          mvprintw(c.y + h, c.x + w, tmp);
        }
      }
    }
    else {
      Coordinate c = objects[i]->get_coordinate(t);
      char tmp[2] = {synbol, '\0'};
      mvprintw(c.y, c.x, tmp); // no camera move
    }
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
  refresh();
  return 0;
}

std::tuple<std::vector<Object *>, int> collision(std::vector<Object *> objects, size_t t) {
  int gained_score = 0;

  for (size_t i = 0; i < objects.size(); i++) {
    for (size_t j = i + 1; j < objects.size(); j++) {
      if (!UNBEATABLE && objects[i]->symbol == PLAYER_SYMBOL && objects[j]->get_tag() == "block") {
        Block *block = dynamic_cast<Block *>(objects[j]);
        Coordinate c = block->get_coordinate(t);
        Coordinate player_coordinate = objects[i]->get_coordinate(t);
        if (player_coordinate.x >= c.x && player_coordinate.x <= c.x + block->width
          && player_coordinate.y >= c.y && player_coordinate.y <= c.y + block->height) {
          game_over();
        }
      }
      else if (objects[i]->get_coordinate(t) == objects[j]->get_coordinate(t)){
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
std::vector<Object *> clean_up(std::vector<Object *> objects, size_t t) {
  int  width, height;
  getmaxyx(stdscr, height, width);
  int margin = 10;
  for (ssize_t i = objects.size() - 1; i >= 0; i--) {
    Coordinate c = objects[i]->get_coordinate(t);
    if (c.y < -margin || c.y >= height || c.x < 0 || c.x >= width + margin) {
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

int main(void) {
  std::vector<Object *> objects;
  size_t frame_tick = 0; // increment along with FLAME_RATE

  // player spawn
  objects.push_back(new Player(10, 10, frame_tick, [](int t) {return Coordinate(-t, 0); }, 'P'));
  initscr();
  noecho(); // キーが入力されても表示しない
  curs_set(0);// カーソルを非表示
  timeout(50);
  srand(time(0));
  int score = 0;
  auto now_time = datetime_millisec();
  while (1) {
    int ch = getch();
    if (ch == 'q') {
      break;
    }
    // spawning
    int  width, height;
    getmaxyx(stdscr, height, width);
    if (frame_tick % SPAWN_PER_TICK == 0 && rand() % 100 <= SPAWN_RATE){ // enemy spawn
      objects.push_back(new Enemy(width - 2, rand() % height - 1, frame_tick, [](int t) {return Coordinate(-t, 0); }, 'X'));
    }
    if (frame_tick % SPAWN_PER_TICK == 0 && rand() % 100 <= SPAWN_RATE){ // block spawn
      int block_width = rand() % 5 + 1;
      int block_height = rand() % 5 + 1;
      objects.push_back(new Block(width - 2 - block_height, rand() % height - 3 - block_height, frame_tick, [](int t) {return Coordinate(-t, 0); }, '#', block_width, block_height));
    }

    // update all objects
    std::vector<Object *> new_objects; // == []
    for (size_t i = 0; i < objects.size(); i++) {
      std::vector<Object *> tmp = objects[i]->update(ch, frame_tick);
      for (size_t j = 0; j < tmp.size(); j++) {
        new_objects.push_back(tmp[j]);
      }
    }
    // merge new_objects to objects
    for (size_t i = 0; i < new_objects.size(); i++) {
      objects.push_back(new_objects[i]);
    }
    // check collision
    auto [new_objs, new_score] = collision(objects, frame_tick);
    objects = new_objs;
    score += new_score;
    // display all objects
    display(objects, score, frame_tick);
    // clean up
    objects = clean_up(objects, frame_tick);
    if (datetime_millisec() - now_time > 1 / FLAME_RATE * 1000) {
      now_time = datetime_millisec();
      frame_tick ++;
    }
  }
}
