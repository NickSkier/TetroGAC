#pragma once

#include "GameField.h"

#define COLOR_ORANGE 8
#define F_WIDTH 10
#define F_HEIGHT 26
#define F_VISIBLE_HEIGHT 20

class GameField;

class Tetromino {
public:
	Tetromino(int x = F_WIDTH / 2 - 2, int y = F_HEIGHT - 4, std::string str = "[]");
	~Tetromino();

	int randomBag();

	void setSymbol(const std::string str);
	std::string getSymbol() const;
	void setType(const int cellValue);
	int getType() const;
	size_t getShape() const;
	size_t getRotation() const;

	void setXY(const int x, const int y);
	int getX() const;
	int getY() const;

	void update(GameField* field, int cellValue = -1);
	bool checkCollisions(const GameField* field) const;

	void rotate(const bool reverse = false);
	bool moveXY(GameField* field, const int changeX = 0, const int changeY = 0);
	int hardDrop(GameField* field);

private:
	int tetrominoX;
	int tetrominoY;
	size_t shape;
	size_t rotation;
	int cellType;
	std::string tetrominoSymbol;
};
