#pragma once

#include "GameField.h"
#include "Tetromino.h"

#define F_WIDTH 10
#define F_HEIGHT 26
#define F_VISIBLE_HEIGHT 20

class Game {
public:
	void refreshField(GameField* field, size_t timeForUpdate = 0);
	void print(GameField* field) const;
	void printBorders(GameField* field) const;

	bool checkLineState(GameField* field, size_t lineNumber, bool checkForFull = true);
	void shiftLines(GameField* field, size_t maxPasses = 4, bool checkType = true);
	int clearAndShiftLines(GameField* field);

	bool GameOver(GameField* field, Tetromino* tetro);
};
