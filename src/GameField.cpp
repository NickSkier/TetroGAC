#include <iostream>
#include <ncurses.h>
#include "GameField.h"

GameField::GameField(size_t width, size_t height, int visWidth, int visHeight, std::string cellString, int cellValue) : width(width), height(height), emptyCell(cellString) {
	if (visWidth == -1) {
		visibleWidth = width;
	}
	else {
		visibleWidth = visWidth;
	}
	if (visHeight == -1) {
		visibleHeight = height;
	}
	else {
		visibleHeight = visHeight;
	}
	field = new int*[width];
	for (size_t i = 0; i < width; ++i) {
		field[i] = new int[height];
	}
	fill(cellValue);
}

GameField::~GameField() {
	for (size_t i = 0; i < width; ++i) {
		delete[] field[i];
	}
	delete[] field;
}

void GameField::fill(const int cellValue) {
	for (size_t i = 0; i < height; ++i) {
		for (size_t j = 0; j < width; ++j) {
			field[j][i] = cellValue;
		}
	}
}

int GameField::getCell(const size_t x, const size_t y) const {
	if (x >= width || y >= height) {
		throw std::out_of_range("Out of range while getting a cell");
	}
	return field[x][y];
}

void GameField::setCell(const size_t x, const size_t y, const int cellValue) {
	if (x >= width || y >= height) {
		throw std::out_of_range("Out of range while setting a cell");
	}
	field[x][y] = cellValue;
}

size_t GameField::getWidth() const {
	return width;
}

size_t GameField::getHeight() const {
	return height;
}

size_t GameField::getVisibleWidth() const {
	return visibleWidth;
}

size_t GameField::getVisibleHeight() const {
	return visibleHeight;
}

std::string GameField::getEmptyCell() const {
	return emptyCell;
}

void GameField::setEmptyCell(const std::string cellString) {
	emptyCell = cellString;
}

int& GameField::operator()(const size_t x, const size_t y) {
	if (x >= width || y >= height) {
	std::cerr << x << " " << y;
		throw std::out_of_range("Out of range while accessing a cell");
	}
	return field[x][y];
}

const int& GameField::operator()(const size_t x, const size_t y) const {
	if (x >= width || y >= height) {
		throw std::out_of_range("Out of range while accessing a cell");
	}
	return field[x][y];
}

