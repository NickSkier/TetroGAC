#pragma once

#include "GameField.h"

class GameField;

class Tetromino {
private:
	int tetrominoX;
	int tetrominoY;
	size_t shape;
	size_t rotation;
	std::string tetrominoSymbol;

public:
	Tetromino(std::string str = "[]");
	~Tetromino();

	void updateField(GameField* field);

	void setSymbol(std::string str);
	std::string getSymbol();
	size_t getShape();
	size_t getRotation();

	void setXY(int& x, int& y);
	int getX() const;
	int getY() const;

	void update(GameField* field, std::string str = "");
	bool colide(GameField* field);

	void rotate(bool reverse = false);
	bool moveXY(GameField* field, int changeX = 0, int changeY = 0);
	void hardDrop(GameField* field);
};
