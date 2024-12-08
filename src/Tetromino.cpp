#include <iostream>
#include <random>
#include <ncurses.h>
#include "Tetromino.h"
#include "GameField.h"
#include "Tetrominoes.h"

Tetromino::Tetromino(std::string str) : tetrominoSymbol(str) {
	tetrominoX = F_WIDTH / 2 - 1;
	tetrominoY = F_VISIBLE_HEIGHT + 2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> shape_gen(0, 6);
    shape = shape_gen(gen);
    rotation = 0;
}

Tetromino::~Tetromino() { }


void Tetromino::setSymbol(std::string str) {
	tetrominoSymbol = str;
}

std::string Tetromino::getSymbol() {
	return tetrominoSymbol;
}

size_t Tetromino::getShape() {
	return shape;
}

size_t Tetromino::getRotation() {
	return rotation;
}

void Tetromino::setXY(int& x, int& y) {
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

void Tetromino::update(GameField* field, std::string str) {
	if (str.empty()) {
        str = tetrominoSymbol;
    }
	for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
        	if (tetrominoes[shape][rotation][j][i]) {
        		if (tetrominoX + i < 0 || tetrominoX + i >= F_WIDTH ||
		    		tetrominoY - j < 0 || tetrominoY - j >= F_HEIGHT)
		    		{ }
        		else  {
					field->setCell(tetrominoX + i, tetrominoY - j, str);
				}
		    }
        }
    }
}

bool Tetromino::colide(GameField* field) {
	for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
        	if (tetrominoes[shape][rotation][j][i]) {
        		if (tetrominoX + i < 0 || tetrominoX + i >= F_WIDTH ||
		    		tetrominoY - j < 0 || tetrominoY - j >= F_HEIGHT) {
		    		return true;
		    	} else if (field->getCell(tetrominoX + i, tetrominoY - j) != field->getEmptyCell()) {
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
	update(field, field->getEmptyCell());
	tetrominoX += changeX;
	tetrominoY += changeY;

	if (colide(field)) {
		tetrominoX -= changeX;
		tetrominoY -= changeY;
		update(field);
		return false;
	}
	update(field);
	return true;
}

void Tetromino::hardDrop(GameField* field) {
	bool reachedBottom = true;
	while (reachedBottom) {
		reachedBottom = moveXY(field, 0, -1);
	}
}
