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

	// init_color(COLOR_GREEN, 0, 500, 0);
	// init_color(COLOR_ORANGE, 1000, 300, 0);
	// init_pair(1, COLOR_CYAN, COLOR_CYAN);
	// init_pair(2, COLOR_BLUE, COLOR_BLUE);
	// init_pair(3, COLOR_ORANGE, COLOR_ORANGE);
	// init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	// init_pair(5, COLOR_GREEN, COLOR_GREEN);
	// init_pair(6, COLOR_RED, COLOR_RED);
	// init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);

	Game game;

	while (game.gameLoop());

	endwin();

	return 0;
}
