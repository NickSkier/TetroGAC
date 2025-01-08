#include <iostream>
#include <string>
#include <ncurses.h>
#include <chrono>
#include "Game.h"
#include "GameField.h"
#include "Tetromino.h"

bool Game::gameLoop() {
    initColors();
	printBorders(&field, true);
	while (gameState) {
		if (!levelLoop()) {
			return false;
		}
		++level;
	}
	endMessage();
	return false;
}

bool Game::levelLoop() {
	int userInput;
	auto timeToMove = std::chrono::steady_clock::now();

	printPreview();
	while (totalClearedLines < (level * 10 + 10)) {
		if (gameState == false) { return false; }
		if (std::chrono::steady_clock::now() >= timeToMove) {
			if (!handleTetrominoLanding(tetromino.moveXY(&field, 0, -1))) {	// Move down and check if a tetromino collided with other blocks
				gameState = false;	// Return false if GameOver is true;
			}
			timeToMove += std::chrono::milliseconds(levelSpeed[level]);
			refreshField(&field);
		}
		userInput = getch();
		if (userInput != ERR) {	// Check if a key has been pressed
			gameControls(userInput);
			refreshField(&field);
		}
	}
	return true;
}

int Game::getLevel() const {
	return level;
}

void Game::setLevel(const int level) {
	this->level = level;
}

void Game::refreshField(const GameField* field, const size_t timeForUpdate, const int offsetX, const int offsetY, const bool borders) const {
	print(field, offsetX, offsetY);
	printStats();
	if (borders) printBorders(field, false, offsetX, offsetY);
	refresh();

	napms(timeForUpdate);
}

void Game::printStats() const {
	mvprintw(8, 28, "Lines: %-*d", 3, totalClearedLines);
	mvprintw(10, 28, "Score: %d", score);
	mvprintw(12, 28, "Level: %d", level);
}

void Game::print(const GameField* field, const int offsetX, const int offsetY) const {
	std::string blockString = "[]";
	int visHeight = field->getVisibleHeight();
	int visWidth = field->getVisibleWidth();
	for (int i = visHeight - 1; i >= 0; --i) {
		for (int j = 0; j < visWidth; ++j) {
			switch((*field)(j, i)) {
			case 0:
				attron(A_BOLD);
				mvprintw(visHeight + offsetY - i, (j * 2) + 5 + offsetX, "%s", field->getEmptyCell().c_str());
				attroff(A_BOLD);
				break;
			default:
				attron(COLOR_PAIR((*field)(j, i)));
				mvprintw(visHeight + offsetY - i, (j * 2) + 5 + offsetX, "%s", blockString.c_str());
				attroff(COLOR_PAIR((*field)(j, i)));
				break;
			}
		}
	}
}

void Game::printBorders(const GameField* field, const bool animate, const int offsetX, const int offsetY) const {
	int visHeight = field->getVisibleHeight();
	int visWidth = field->getVisibleWidth();

	int bottomBorderY = visHeight + 1 + offsetY;
	int leftHorizontalBorderStart = visWidth + 5 + offsetX;
	int rightHorizontalBorderStart = visWidth + 4 + offsetX;
	int leftBorderX1 = 3 + offsetX;
	int leftBorderX2 = 4 + offsetX;
	int rightBorderX1 = visWidth * 2 + 5 + offsetX;
	int rightBorderX2 = visWidth * 2 + 6 + offsetX;

	std::string checkStr;

	attron(A_BOLD);
	for (int i = 0; i < visWidth; ++i) {
		if (mvinch(bottomBorderY, leftHorizontalBorderStart + i) != '=') {
			mvprintw(bottomBorderY, leftHorizontalBorderStart + i, "=");
		}
		if (mvinch(bottomBorderY, rightHorizontalBorderStart - i) != '=') {
			mvprintw(bottomBorderY, rightHorizontalBorderStart - i, "=");
		}
		if (animate) {
			refresh();
			napms(20);
		}
	}

	for (int i = bottomBorderY; i > 0 ; --i) {
		if (mvinch(i, leftBorderX1) != '<') {
			mvprintw(i, leftBorderX1, "<");
		}
		if (mvinch(i, leftBorderX2) != '!') {
			mvprintw(i, leftBorderX2, "!");
		}
		if (mvinch(i, rightBorderX1) != '!') {
			mvprintw(i, rightBorderX1, "!");
		}
		if (mvinch(i, rightBorderX2) != '>') {
			mvprintw(i, rightBorderX2, ">");
		}
		if (animate) {
			refresh();
			napms(20);
		}
	}
	attroff(A_BOLD);
}

void Game::printPreview() {
	preview.fill();
	Tetromino nextTetrominoPreview = nextTetromino;
	nextTetrominoPreview.setXY(1, 3);
	nextTetrominoPreview.update(&preview);
	refreshField(&preview, 0, 23, 0, false);
}

bool Game::handleTetrominoLanding(const bool collided) {
	if (!collided) {	// Move down and check if a tetromino collided with other blocks
		clearLinesAndUpdateScore();
		if (GameOver(&field, &tetromino)) {
			return false;
		}
		tetromino = nextTetromino;
		nextTetromino = Tetromino();
		printPreview();
	}
	return true;
}

void Game::clearLinesAndUpdateScore() {
	int linesCleared = clearAndShiftLines(&field);	// Clear full lines

	if (linesCleared) {
		switch (linesCleared) {
			case 1: score += 100; break;	// 100 points for 1 full line
			case 2: score += 300; break;	// 300 points for 2 full lines
			case 3: score += 500; break;	// 500 points for 3 full lines
			case 4: score += 800; break;	// 800 points for 4 full lines
		}
	}

	totalClearedLines += linesCleared;
}

bool Game::checkLineState(const GameField* field, const size_t lineNumber, const bool checkForFull) const {
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

void Game::shiftLines(GameField* field, const size_t maxPasses, const bool checkType) {
/*
 * maxPasses - The number of iterations over the array.
 * The maximum value is 4; more passes are unnecessary
 * because of the Tetris logic.
 */
	int fieldVisHeight = field->getVisibleHeight();
	int fieldWidth = field->getWidth();
	int norefresh = 0;
	for (size_t lineNumber = 0; lineNumber < maxPasses; ++lineNumber) {
		for (int lineNumber = 0; lineNumber < fieldVisHeight; ++lineNumber) {	// Traverse visible lines
			if (checkLineState(field, lineNumber, checkType)) {
				for (int j = lineNumber; j < fieldVisHeight; ++j) {				// Shift lines down from this point
					for (int k = 0; k < fieldWidth; ++k) {						// Traverse cells in the current line
						if ((*field)(k, j) != (*field)(k, j + 1)) {				// Avoid redundant copying
							(*field)(k, j) = (*field)(k, j + 1);				// Copy cell data from the line above
						}
						else {
							++norefresh;
						}
					}
					if (norefresh < fieldWidth) refreshField(field, 15);		// Refresh the field to animate lines shifting
					norefresh = 0;
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
		if (checkLineState(field, lineNumber, true)) {									// Find full lines
			++linesClearedCounter;														// Track the number of cleared lines
			for (size_t j = 0; j < field->getWidth()/2; ++j) {							// Clear each cell in the full line
				if ((*field)(j, lineNumber) != 0) {										// Avoid redundant clearing
					(*field)(j, lineNumber) = 0;										// Mark the cell as empty from left part of the line
				}
				if ((*field)(field->getWidth()-1-j, lineNumber) != 0) {					// Avoid redundant clearing
					(*field)(field->getWidth()-1-j, lineNumber) = 0;					// Mark the cell as empty from right part of the line
				}
			refreshField(field, 30);		// Refresh the field to animate cells clearing (delay: 30 ms)
			}
		}
	}

	// Shift lines down if any were cleared
	if (linesClearedCounter != 0) {
		shiftLines(field, linesClearedCounter, false);	// Shift empty lines
	}
	return linesClearedCounter;
}

void Game::endMessage() const {
	std::string message1 = "All lines cleared!";
	std::string message2 = "Your score: " + std::to_string(score);
	attron(A_BOLD);
	mvprintw(F_VISIBLE_HEIGHT / 2, 5 + F_WIDTH - message1.size() / 2, "%s", message1.c_str());
	mvprintw(F_VISIBLE_HEIGHT / 2 + 1, 5 + F_WIDTH - message2.size() / 2, "%s", message2.c_str());
	refresh();
	attroff(A_BOLD);
	getchar();
}

bool Game::GameOver(const GameField* field, const Tetromino* tetro) const {
	if (tetro->checkCollisions(field) && tetro->getY() >= F_VISIBLE_HEIGHT) {
		refreshField(field);
		GameOverAnimation();
		return true;
	}
	return false;
}

void Game::GameOverAnimation() const {
	const std::string gameOverMessage = "Game Over!";
	bool shouldBrake = false;
	beep(); // Ring a terminal bell to notify a player about his Game Over;
	attron(A_BOLD);
	mvprintw(F_VISIBLE_HEIGHT / 2, 5 + F_WIDTH - gameOverMessage.size() / 2, "%s", gameOverMessage.c_str());
	refresh();
	attroff(A_BOLD);
	timeout(-1);
	while (getch() != 10) {}
	flushinp();
	timeout(1);
	for (size_t i = 0; i < F_VISIBLE_HEIGHT + 5; ++i) {
		if (getch() != ERR) {
			shouldBrake = true;
			break;
		}
		for (size_t j = 0; j < F_WIDTH * 2 + 20; ++j) {
			if (!(i == F_VISIBLE_HEIGHT / 2 && j >= F_WIDTH && j < F_WIDTH + gameOverMessage.length())) {
				mvprintw(i, j, " ");
			}
		}
		refresh();
		napms(60);
		if (shouldBrake) break;
	}
}

void Game::gameControls(const int userInput) {
	switch (userInput) {
		case (int)'q':	// Quit the game
			endwin();
			std::cout << "\nStopped by user.\n\n";
			std::exit(0);
		case KEY_LEFT:	// Move Left
			tetromino.moveXY(&field, -1, 0);
			break;
		case KEY_RIGHT:	// Move Right
			tetromino.moveXY(&field, 1, 0);
			break;
		case KEY_DOWN:	// Soft Drop
			if (tetromino.moveXY(&field, 0, -1)) {
				score += 1;	// 1 point for each cell of a soft drop
			}
			break;
		case (int)' ':	// Hard Drop
			score += tetromino.hardDrop(&field); // 2 points for each cell of a hard drop
			if (!handleTetrominoLanding(false)) {gameState = false;}
			break;
		case KEY_UP:	// Rotate Clockwise
			tetromino.update(&field, 0);
			tetromino.rotate();
			if (tetromino.checkCollisions(&field)) tetromino.rotate(true);
			break;
		case (int)'e':	// Rotate Counterclockwise
			tetromino.update(&field, 0);
			tetromino.rotate(true);
			if (tetromino.checkCollisions(&field)) tetromino.rotate();
			break;
		case (int)'l':	// Increase totalClearedLines
			++totalClearedLines;
			break;
		}
	tetromino.update(&field);
}

void Game::initColors() {
	init_color(COLOR_GREEN, 0, 500, 0);
	init_color(COLOR_ORANGE, 1000, 300, 0);
	init_pair(1, COLOR_CYAN, COLOR_CYAN);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_ORANGE, COLOR_ORANGE);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(5, COLOR_GREEN, COLOR_GREEN);
	init_pair(6, COLOR_RED, COLOR_RED);
	init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
}
