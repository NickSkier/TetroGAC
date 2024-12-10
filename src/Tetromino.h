#pragma once

#include "GameField.h"

#define COLOR_ORANGE 8
#define F_WIDTH 10
#define F_HEIGHT 26
#define F_VISIBLE_HEIGHT 20

class GameField;

class Tetromino {
private:
	int tetrominoX;
	int tetrominoY;
	size_t shape;
	size_t rotation;
	int cellType;
	std::string tetrominoSymbol;

public:
	Tetromino(int x = F_WIDTH / 2 - 1, int y = F_HEIGHT - 4, std::string str = "[]");
	~Tetromino();

	Tetromino& operator=(const Tetromino & other);

	int randomBag();

	void setSymbol(std::string str);
	std::string getSymbol();
	void setType(int cellValue);
	int getType();
	size_t getShape();
	size_t getRotation();

	void setXY(int x, int y);
	int getX() const;
	int getY() const;

	void update(GameField* field, int cellValue = -1);
	bool checkCollisions(GameField* field);

	void rotate(bool reverse = false);
	bool moveXY(GameField* field, int changeX = 0, int changeY = 0);
	int hardDrop(GameField* field);
};
