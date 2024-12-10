#include <iostream>
#include <random>
#include <ctime>
#include <ncurses.h>
#include "Tetromino.h"
#include "Game.h"
#include "GameField.h"
#include "Tetrominoes.h"

Tetromino::Tetromino(int x, int y, std::string str) : tetrominoSymbol(str) {
	tetrominoX = x;
	tetrominoY = y;
    shape = randomBag();
    cellType = shape + 1;
    rotation = 0;
}

Tetromino::~Tetromino() { }

Tetromino& Tetromino::operator=(const Tetromino & other) {
	if (this != &other) {
		tetrominoX = other.tetrominoX;
		tetrominoY = other.tetrominoY;
		shape = other.shape;
		rotation = other.rotation;
		cellType = other.cellType;
		tetrominoSymbol = other.tetrominoSymbol;
	}
	return *this;
}

int Tetromino::randomBag() {
	static int bag[7] = { 0, 1, 2, 3, 4, 5, 6 };
    static int bagIterator = 7;
    static std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));

    if (bagIterator == 7) {
        for (int i = 6; i > 0; --i) {
            std::uniform_int_distribution<> dist(0, i);
            int j = dist(gen);
            std::swap(bag[i], bag[j]);
        }
        bagIterator = 0;
    }

    return bag[bagIterator++];
}


void Tetromino::setSymbol(std::string str) {
	tetrominoSymbol = str;
}

std::string Tetromino::getSymbol() {
	return tetrominoSymbol;
}

void Tetromino::setType(int cellValue) {
	cellType = cellValue;
}

int Tetromino::getType() {
	return cellType;
}

size_t Tetromino::getShape() {
	return shape;
}

size_t Tetromino::getRotation() {
	return rotation;
}

void Tetromino::setXY(int x, int y) {
	if (x < 0 || x >= F_WIDTH || y < 0 || y >= F_HEIGHT) {
		throw std::out_of_range("Out of range while setting a tetromino's XY coordinates");
	}
	tetrominoX = x;
	tetrominoY = y;
}

int Tetromino::getX() const {
	return tetrominoX;
}

int Tetromino::getY() const {
	return tetrominoY;
}

void Tetromino::update(GameField* field, int cellValue) {
	if (cellValue == -1) {
        cellValue = cellType;
    }
	for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
        	if (tetrominoes[shape][rotation][j][i]) {
        		if (tetrominoX + i < 0 || tetrominoX + i >= field->getWidth() ||
		    		tetrominoY - j < 0 || tetrominoY - j >= field->getHeight())
		    		{ }
        		else  {
					field->setCell(tetrominoX + i, tetrominoY - j, cellValue);
				}
		    }
        }
    }
}

bool Tetromino::checkCollisions(GameField* field) {
	for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
        	if (tetrominoes[shape][rotation][j][i]) {
        		if (tetrominoX + i < 0 || tetrominoX + i >= field->getWidth() ||
		    		tetrominoY - j < 0 || tetrominoY - j >= field->getHeight()) {
		    		return true;
		    	} else if (field->getCell(tetrominoX + i, tetrominoY - j) != 0) {
	    			return true;
	    		}
		    }
        }
    }
    return false;
}


void Tetromino::rotate(bool reverse) {
	if (reverse) rotation += 2;
	rotation = (rotation + 1) % 4;
}

bool Tetromino::moveXY(GameField* field, int changeX, int changeY) {
	update(field, 0);
	tetrominoX += changeX;
	tetrominoY += changeY;

	if (checkCollisions(field)) {
		tetrominoX -= changeX;
		tetrominoY -= changeY;
		update(field);
		return false;
	}
	update(field);
	return true;
}

int Tetromino::hardDrop(GameField* field) {
	int score = 0;
	bool reachedBottom = true;
	while (reachedBottom) {
		reachedBottom = moveXY(field, 0, -1);
		score += 2;
	}
	return score;
}
