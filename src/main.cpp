#include <iostream>
#include <ncurses.h>
#include "Game.h"
#include "GameField.h"
#include "Tetromino.h"

int main() {
	const int numberOfTetrominos = F_WIDTH * F_HEIGHT;
	int tetrominoCounter = numberOfTetrominos - 1;
	int score = 0;
	int scoreLinesCounter;
	int userInput;
	int tickCounter = 0;

	Game game;
	GameField field;
	Tetromino tetro[numberOfTetrominos];

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

	// Field borders
	attron(A_BOLD);
    for (size_t i = 0; i < F_WIDTH; ++i) {
		mvprintw(F_VISIBLE_HEIGHT + 1, F_WIDTH + i + 5, "=");
		mvprintw(F_VISIBLE_HEIGHT + 1, F_WIDTH - i + 4, "=");
		refresh();
		//napms(20);
	}
    for (int i = F_VISIBLE_HEIGHT+1; i > 0 ; --i) {
        mvprintw(i, 3, "<!");
        mvprintw(i, F_WIDTH * 2 + 5, "!>");
		refresh();
		//napms(20);
	}
	for (int i = F_VISIBLE_HEIGHT; i > 0; --i) {
		for (size_t j = 0; j < F_WIDTH; ++j) {
			mvprintw(i, (j * 2)+5, "%s", field.getEmptyCell().c_str());
			refresh();
			//napms(7);
		}
	}
	attroff(A_BOLD);

	// Game loop
	try {
		while (tetrominoCounter >= 0) {
			napms(16);
    		tickCounter++;
		    mvprintw(0, 0, "Tetrominoes left: %d", tetrominoCounter);
		    mvprintw(0, 25, "Score: %d", score);
			if (tickCounter % 30 == 0) {
				if (!tetro[tetrominoCounter].moveXY(&field, 0, -1)) {	// Move down and check if a tetromino collided with other blocks

					scoreLinesCounter = field.clearAndShiftLines();	// Clear full lines

					if (scoreLinesCounter) {
						switch (scoreLinesCounter) {
						case 1: score += 100; break;	// 100 points for 1 full line
						case 2: score += 300; break;	// 300 points for 2 full lines
						case 3: score += 500; break;	// 500 points for 3 full lines
						case 4: score += 800; break;	// 800 points for 4 full lines
						}
					}

					if (game.GameOver(&field, &tetro[tetrominoCounter])) {
						break;
					}

					--tetrominoCounter;
				}
				field.refreshField();
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
					tetro[tetrominoCounter].moveXY(&field, -1, 0);
					break;
				case KEY_RIGHT:	// Move Right
					tetro[tetrominoCounter].moveXY(&field, 1, 0);
					break;
				case KEY_DOWN:	// Soft Drop
					tetro[tetrominoCounter].moveXY(&field, 0, -1);
					score += 1;	// 1 point for each cell of a soft drop
					break;
				case (int)' ':	// Hard Drop
					score += tetro[tetrominoCounter].hardDrop(&field); // 2 points for each cell of a hard drop
					break;
				case KEY_UP:	// Rotate Clockwise
					tetro[tetrominoCounter].update(&field, 0);
					tetro[tetrominoCounter].rotate();
					if (tetro[tetrominoCounter].checkCollisions(&field)) {
						tetro[tetrominoCounter].rotate(true);
					}
					tetro[tetrominoCounter].update(&field);
					break;
				case (int)'e':	// Rotate Counterclockwise
					tetro[tetrominoCounter].update(&field, 0);
					tetro[tetrominoCounter].rotate(true);
					if (tetro[tetrominoCounter].checkCollisions(&field)) {
						tetro[tetrominoCounter].rotate();
					}
					tetro[tetrominoCounter].update(&field);
					break;
				}
				field.refreshField();
			}
		}
	}
	catch (const std::exception& e) {
		mvprintw(F_VISIBLE_HEIGHT + 1, 0, "Error: %s", e.what());
		refresh();
		getchar();
	}

    endwin();

    if (tetrominoCounter == 0) {
        std::cout << "\nThere are no more tetrominoes.\n\n";
    }

	return 0;
}
