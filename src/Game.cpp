#include <string>
#include <ncurses.h>
#include "Game.h"
#include "GameField.h"
#include "Tetromino.h"


void Game::refreshField(GameField* field, size_t timeForUpdate) {
    static int frameCounter = 0;

    print(field);
    printBorders(field);
    refresh();

    ++frameCounter;
    mvprintw(field->getVisibleHeight() + 2, 0, "Frame: %d", frameCounter);

    napms(timeForUpdate);
}

void Game::print(GameField* field) const {
std::string blockString = "[]";
	for (int i = field->getVisibleHeight() - 1; i >= 0; --i) {
		for (int j = 0; j < field->getWidth(); ++j) {
			switch((*field)(j, i)) {
			case 0:
				attron(A_BOLD);
				mvprintw(field->getVisibleHeight() - i, (j * 2)+5, "%s", field->getEmptyCell().c_str());
				attroff(A_BOLD);
				break;
			default:
				attron(COLOR_PAIR((*field)(j, i)));
				mvprintw(field->getVisibleHeight() - i, (j * 2)+5, "%s", blockString.c_str());
				attroff(COLOR_PAIR((*field)(j, i)));
				break;
			}
		}
	}
}

void Game::printBorders(GameField* field) const {
	attron(A_BOLD);
    for (size_t i = 0; i < field->getWidth(); ++i) {
		mvprintw(field->getVisibleHeight() + 1, field->getWidth() + i + 5, "=");
		mvprintw(field->getVisibleHeight() + 1, field->getWidth() - i + 4, "=");
		refresh();
	}
    for (int i = field->getVisibleHeight()+1; i > 0 ; --i) {
        mvprintw(i, 3, "<!");
        mvprintw(i, field->getWidth() * 2 + 5, "!>");
		refresh();
	}
	attroff(A_BOLD);
}

bool Game::checkLineState(GameField* field, size_t lineNumber, bool checkForFull) {
	for (size_t i = 0; i < field->getWidth(); ++i) {
		if (checkForFull) {
			if ((*field)(i, lineNumber) == 0) {
				return false;
			}
		}
		else {
			if ((*field)(i, lineNumber) != 0) {
				return false;
			}
		}
	}
	return true;
}

void Game::shiftLines(GameField* field, size_t maxPasses, bool checkType) {
/*
 * maxPasses - The number of iterations over the array.
 * The maximum value is 4; more passes are unnecessary
 * because of the Tetris logic.
 */
 	bool norefresh = false;
	for (size_t lineNumber = 0; lineNumber < maxPasses; ++lineNumber) {
		for (size_t lineNumber = 0; lineNumber < field->getVisibleHeight(); ++lineNumber) { // Traverse visible lines
			if (checkLineState(field, lineNumber, checkType)) {
				for (size_t j = lineNumber; j < field->getVisibleHeight(); ++j) {// Shift lines down from this point
						for (size_t k = 0; k < field->getWidth(); ++k) {			// Traverse cells in the current line
							if ((*field)(k, j) != (*field)(k, j + 1)) {		// Avoid redundant copying
								(*field)(k, j) = (*field)(k, j + 1);			// Copy cell data from the line above
								norefresh = false;
							}
							else {
								norefresh = true;
							}

					}
					if (!norefresh) refreshField(field, 30);					// Refresh the field to animate lines shifting
				}
			}
		}
	}
}

// Check for full lines, if any clear them and shift remaining lines down
int Game::clearAndShiftLines(GameField* field) {
	size_t linesClearedCounter = 0;

	// Clear full lines
	for (size_t lineNumber = 0; lineNumber < field->getVisibleHeight(); ++lineNumber) {	// Traverse visible lines
		if (checkLineState(field, lineNumber, true)) {			 	// Find full lines
			++linesClearedCounter;					 	 	// Track the number of cleared lines
			for (size_t j = 0; j < field->getWidth()/2; ++j) { 	 	// Clear each cell in the full line
				if ((*field)(j, lineNumber) != 0) { 		 	// Avoid redundant clearing
					(*field)(j, lineNumber) = 0; 	 		 	// Mark the cell as empty from left part of the line
				}
				if ((*field)(field->getWidth()-1-j, lineNumber) != 0) { 	// Avoid redundant clearing
					(*field)(field->getWidth()-1-j, lineNumber) = 0; 	// Mark the cell as empty from right part of the line
				}
			refreshField(field, 20);					 		 	// Refresh the field to animate cells clearing (delay: 30 ms)
			}
		}
	}

	// Shift lines down if any were cleared
	if (linesClearedCounter != 0) {
		shiftLines(field, linesClearedCounter, false);	// Shift empty lines
	}
	return linesClearedCounter;
}

bool Game::GameOver(GameField* field, Tetromino* tetro) {
	std::string gameOverMessage = "Game Over!";
	int userInterrupt;
	bool shouldBrake = false;

	if (tetro->checkCollisions(field) && tetro->getY() > field->getVisibleHeight()) { // The whole logic of the Game Over
		// Everything lower is just a message and an animation of exiting
		attron(A_BOLD);
		mvprintw(field->getVisibleHeight() / 2, field->getWidth(), "%s", gameOverMessage.c_str());
		refresh();
		attroff(A_BOLD);
		getchar();
		for (size_t i = 0; i < field->getHeight() + 5; ++i) {
			for (size_t j = 0; j < field->getWidth() * 2 + 15; ++j) {
				userInterrupt = getch();
				if (userInterrupt != ERR) {
					shouldBrake = true;
					break;
				}
				else {
					if (!(i == field->getVisibleHeight() / 2 && j >= field->getWidth() && j < field->getWidth() + gameOverMessage.length())) {
						mvprintw(i, j, " ");
						refresh();
						napms(2);
					}
				}
			}
			if (shouldBrake) break;
		}
		return true;
	}
	return false;
}
