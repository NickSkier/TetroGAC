#include <ncurses.h>
#include "Game.h"

int main() {

	// Initialize ncurses
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	cbreak();
	nodelay(stdscr, TRUE);
	start_color();

	Game game;

	while (game.gameLoop());

	endwin();

	return 0;
}
