#pragma once

#include <string>
#include "Tetromino.h"

class Tetromino;

class GameField {
private:
	int** field;
    size_t width;
    size_t height;
    int visibleWidth;
    int visibleHeight;
	int cellType;
	std::string emptyCell;

public:
	GameField(size_t width, size_t height, int visWidth = -1, int visHeight = -1, std::string cellString = "  ", int cellValue = 0);
    ~GameField();

	void fill(int cellValue = 0);

    void refreshField(size_t timeForUpdate);

    int getCell(size_t x, size_t y) const;
    void setCell(size_t x, size_t y, int cellValue);

	size_t getWidth() const;
	size_t getHeight() const;
	size_t getVisibleWidth() const;
	size_t getVisibleHeight() const;

    std::string getEmptyCell() const;
    void setEmptyCell(std::string cellString);

    int& operator()(size_t x, size_t y);
    const int& operator()(size_t x, size_t y) const;
};
