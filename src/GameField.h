#pragma once

#include <string>
#include "Tetromino.h"

#define F_WIDTH 10
#define F_HEIGHT 26
#define F_VISIBLE_HEIGHT 20

class Tetromino;

class GameField {
private:
	int field[F_WIDTH][F_HEIGHT];
	int cellType;
	std::string emptyCell;

	void fill(int cellValue = 0);
	void print();

public:
	GameField(std::string cellString = "  ", int cellValue = 0);
	~GameField();

	void refreshField(size_t timeForUpdate = 0);

	int getCell(size_t x, size_t y) const;
	void setCell(size_t x, size_t y, int cellValue);

	std::string getEmptyCell() const;
	void setEmptyCell(std::string cellString);

	bool checkLineState(size_t lineNumber, bool checkForFull = true);
	void shiftLines(size_t maxPasses = 4, bool checkType = true);
	void clearAndShiftLines();
};

