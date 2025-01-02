#pragma once

#include <string>
#include "Tetromino.h"

class Tetromino;

class GameField {
public:
	GameField(size_t width, size_t height, int visWidth = -1, int visHeight = -1, std::string cellString = "  ", int cellValue = 0);
	~GameField();

	int& operator()(const size_t x, const size_t y);
	const int& operator()(const size_t x, const size_t y) const;

	void fill(const int cellValue = 0);

	int getCell(const size_t x, const size_t y) const;
	void setCell(const size_t x, const size_t y, const int cellValue);

	size_t getWidth() const;
	size_t getHeight() const;
	size_t getVisibleWidth() const;
	size_t getVisibleHeight() const;

	std::string getEmptyCell() const;
	void setEmptyCell(const std::string cellString);

private:
	int** field;
	size_t width;
	size_t height;
	int visibleWidth;
	int visibleHeight;
	int cellType;
	std::string emptyCell;
};
