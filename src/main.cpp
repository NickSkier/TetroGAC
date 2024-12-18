#include <iostream>
#include <ncurses.h>
#include "Game.h"
#include "GameField.h"
#include "Tetromino.h"

int main() {
	int score = 0;
	int linesCounter;
	int linesToClear = 26;
	int userInput;
	int tickCounter = 0;

	Game game;
	GameField field(F_WIDTH, F_HEIGHT, F_WIDTH , F_VISIBLE_HEIGHT);
	GameField preview(6, 6, -1, -1);
	Tetromino tetromino;
	Tetromino nextTetromino;
	Tetromino nextTetrominoPreview;

    // Initialize ncurses
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
	start_color();

	init_color(COLOR_GREEN, 0, 500, 0);
	init_color(COLOR_ORANGE, 1000, 300, 0);
	init_pair(1, COLOR_CYAN, COLOR_CYAN);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_ORANGE, COLOR_ORANGE);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(5, COLOR_GREEN, COLOR_GREEN);
	init_pair(6, COLOR_RED, COLOR_RED);
	init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);

	// Field borders initialization animation
	attron(A_BOLD);
    for (size_t i = 0; i < F_WIDTH; ++i) {
		mvprintw(F_VISIBLE_HEIGHT + 1, F_WIDTH + i + 5, "=");
		mvprintw(F_VISIBLE_HEIGHT + 1, F_WIDTH - i + 4, "=");
		refresh();
		napms(20);
	}
    for (int i = F_VISIBLE_HEIGHT+1; i > 0 ; --i) {
        mvprintw(i, 3, "<!");
        mvprintw(i, F_WIDTH * 2 + 5, "!>");
		refresh();
		napms(20);
	}
	attroff(A_BOLD);

	nextTetrominoPreview = nextTetromino;
	nextTetrominoPreview.setXY( 1, 3);
	nextTetrominoPreview.update(&preview);
	game.refreshField(&preview, 0, 25, 0, false);

	// Game loop
	try {
		while (linesToClear > 0) {
			napms(16);
    		tickCounter++;
		    mvprintw(0, 0, "Lines left: %*d", 2, linesToClear);
		    mvprintw(0, 20, "Score: %d", score);

			if (tickCounter % 30 == 0) {
				if (!tetromino.moveXY(&field, 0, -1)) {	// Move down and check if a tetromino collided with other blocks

					linesCounter = game.clearAndShiftLines(&field);	// Clear full lines

					if (linesCounter) {
						switch (linesCounter) {
						case 1: score += 100; break;	// 100 points for 1 full line
						case 2: score += 300; break;	// 300 points for 2 full lines
						case 3: score += 500; break;	// 500 points for 3 full lines
						case 4: score += 800; break;	// 800 points for 4 full lines
						}
					}

					linesToClear -= linesCounter;

					if (game.GameOver(&field, &tetromino)) {
						break;
					}

					tetromino = nextTetromino;
					nextTetromino = Tetromino();

					preview.fill();
					nextTetrominoPreview = nextTetromino;
					nextTetrominoPreview.setXY( 1, 3);
					nextTetrominoPreview.update(&preview);
					game.refreshField(&preview, 0, 25, 0, false);
				}
				game.refreshField(&field);
			}
		    userInput = getch();
			if (userInput != ERR) {	// Check if a key has been pressed
				switch (userInput) {
				case (int)'q':	// Quit the game
				    refresh();
					endwin();
					std::cout << "\nStopped by user.\n";
					return 0;
				case KEY_LEFT:	// Move Left
					tetromino.moveXY(&field, -1, 0);
					break;
				case KEY_RIGHT:	// Move Right
					tetromino.moveXY(&field, 1, 0);
					break;
				case KEY_DOWN:	// Soft Drop
					tetromino.moveXY(&field, 0, -1);
					score += 1;	// 1 point for each cell of a soft drop
					break;
				case (int)' ':	// Hard Drop
					score += tetromino.hardDrop(&field); // 2 points for each cell of a hard drop
					break;
				case KEY_UP:	// Rotate Clockwise
					tetromino.update(&field, 0);
					tetromino.rotate();
					if (tetromino.checkCollisions(&field)) {
						tetromino.rotate(true);
					}
					tetromino.update(&field);
					break;
				case (int)'e':	// Rotate Counterclockwise
					tetromino.update(&field, 0);
					tetromino.rotate(true);
					if (tetromino.checkCollisions(&field)) {
						tetromino.rotate();
					}
					tetromino.update(&field);
					break;
				case (int)'l':	// Increase linesToClear
					++linesToClear;
					break;
				}
				game.refreshField(&field);
			}
		}
	}
	catch (const std::exception& e) {
		mvprintw(F_VISIBLE_HEIGHT + 1, 0, "Error: %s", e.what());
		refresh();
		getchar();
	}

    endwin();

    if (linesToClear <= 0) {
        std::cout << std::endl << "You clear all lines!" << std::endl;
        std::cout << "Your score: " << score << std::endl << std::endl;
    }

	return 0;
}

