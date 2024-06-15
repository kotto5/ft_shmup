#include <ncurses.h>
#include <stdio.h>

struct Coordinate {
    public:
    int x;
    int y;
    Coordinate(int x, int y) {
    this->x = x;
    this->y = y;
    }
};

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
  clear();
  mvprintw(player.y, player.x, "P");
  refresh();
  return player;
}

int main(void) {
  Coordinate player(0, 0);
  initscr();
  while (1) {
    int ch = getch();
    if (ch == 'q') {
      break;
    }
    player = update(player, ch);
  }
  endwin();
}
