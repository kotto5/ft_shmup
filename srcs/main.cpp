#include <ncurses.h>
#include <stdio.h>
#include <vector>

#define PLAYER_SYMBOL "(^_^)"
#define ENEMY_SYMBOL "(X_X)"

struct Coordinate {
    public:
    int x;
    int y;
    Coordinate(int x, int y) {
    this->x = x;
    this->y = y;
    }
};

Coordinate update(Coordinate player, std::vector<Coordinate> enemies, int ch) {
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
  clear();
  mvprintw(player.y, player.x, PLAYER_SYMBOL);
  for (size_t i = 0; i < enemies.size(); i++) {
	mvprintw(enemies[i].y, enemies[i].x, ENEMY_SYMBOL);
  }
  refresh();
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

int main(void) {
  Coordinate player(0, 0);
  std::vector<Coordinate> enemies;
  enemies.push_back(Coordinate(10, 10));
  enemies.push_back(Coordinate(20, 20));
  initscr();
  while (1) {
    int ch = getch();
    if (ch == 'q') {
      break;
    }
    player = update(player, enemies, ch);
	if (collide(player, enemies))
		return game_over();
  }
  endwin();
}
