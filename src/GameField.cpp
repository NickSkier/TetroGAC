#include <iostream>
#include <ncurses.h>
#include "GameField.h"

GameField::GameField(std::string cellString, int cellValue) {
	emptyCell = cellString;
	fill(cellValue);
}

GameField::~GameField() { }

void GameField::fill(int cellValue) {
	for (size_t i = 0; i < F_HEIGHT; ++i) {
		for (size_t j = 0; j < F_WIDTH; ++j) {
			field[j][i] = cellValue;
		}
	}
}

void GameField::print() {
std::string cellString = "[]";
	for (int i = F_VISIBLE_HEIGHT - 1; i >= 0; --i) {
		for (size_t j = 0; j < F_WIDTH; ++j) {
			if (field[j][i] == 0) {
				mvprintw(F_VISIBLE_HEIGHT - i, (j * 2)+5, "%s", emptyCell.c_str());
			}
			else {
				mvprintw(F_VISIBLE_HEIGHT - i, (j * 2)+5, "%s", cellString.c_str());
			}
		}
	}
}

void GameField::refreshField(size_t timeForUpdate) {
    static int frameCounter = 0;

    print();
    refresh();

    ++frameCounter;
    mvprintw(F_VISIBLE_HEIGHT + 2, 0, "Frame: %d", frameCounter);

    napms(timeForUpdate);
}

int GameField::getCell(size_t x, size_t y) const {
	if (x < 0 || x >= F_WIDTH || y < 0 || y >= F_HEIGHT) {
		throw std::out_of_range("Out of range while getting a cell");
	}
	else {
		return field[x][y];
	}
}

void GameField::setCell(size_t x, size_t y, int cellValue) {
	if (x < 0 || x >= F_WIDTH || y < 0 || y >= F_HEIGHT) {
		throw std::out_of_range("Out of range while setting a cell");
	}
	else {
		field[x][y] = cellValue;
	}
}

std::string GameField::getEmptyCell() const {
	return emptyCell;
}

void GameField::setEmptyCell(std::string cellString) {
	emptyCell = cellString;
}

bool GameField::checkLineState(size_t lineNumber, bool checkForFull) {
	for (size_t i = 0; i < F_WIDTH; ++i) {
		if (checkForFull) {
			if (field[i][lineNumber] == 0) {
				return false;
			}
		}
		else {
			if (field[i][lineNumber] != 0) {
				return false;
			}
		}
	}

	return true;
}

void GameField::shiftLines(size_t maxPasses, bool checkType) {
/*
 * maxPasses - The number of iterations over the array.
 * The maximum value is 4; more passes are unnecessary
 * because of the Tetris logic.
 */
	for (size_t lineNumber = 0; lineNumber < maxPasses; ++lineNumber) {
		for (size_t lineNumber = 0; lineNumber < F_VISIBLE_HEIGHT; ++lineNumber) { // Traverse visible lines
			if (checkLineState(lineNumber, checkType)) {
				for (size_t j = lineNumber; j < F_VISIBLE_HEIGHT; ++j) {// Shift lines down from this point
						for (size_t k = 0; k < F_WIDTH; ++k) {			// Traverse cells in the current line
							if (field[k][j] != field[k][j + 1]) {		// Avoid redundant copying
								field[k][j] = field[k][j + 1];			// Copy cell data from the line above
								refreshField(10);						// Refresh the field to animate lines shifting
						}
					}
				}
			}
		}
	}
}

// Check for full lines, if any clear them and shift remaining lines down
void GameField::clearAndShiftLines() {
	size_t linesClearedCounter = 0;

	// Clear full lines
	for (size_t lineNumber = 0; lineNumber < F_VISIBLE_HEIGHT; ++lineNumber) {	// Traverse visible lines
		if (checkLineState(lineNumber, true)) {			 // Find full lines
			++linesClearedCounter;					 	 // Track the number of cleared lines
			for (size_t j = 0; j < F_WIDTH; ++j) { 	 	 // Clear each cell in the full line
				if (field[j][lineNumber] != 0) { // Avoid redundant clearing
					field[j][lineNumber] = 0; 	 // Mark the cell as empty
					refreshField(10);					 // Refresh the field to animate cells clearing (delay: 10 ms)
				}
			}
		}
	}

	// Shift lines down if any were cleared
	if (linesClearedCounter != 0) {
		shiftLines(linesClearedCounter, false);	// Shift empty lines
	}
}
