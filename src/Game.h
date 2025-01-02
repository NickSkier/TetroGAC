#pragma once

#include "GameField.h"
#include "Tetromino.h"

#define F_WIDTH 10
#define F_HEIGHT 26
#define F_VISIBLE_HEIGHT 20

class Game {
public:
	Game():field(F_WIDTH, F_HEIGHT, F_WIDTH , F_VISIBLE_HEIGHT), preview(6, 6, -1, -1), tetromino(), nextTetromino(), score(0) {};

	bool gameLoop();

	void refreshField(const GameField* field, const size_t timeForUpdate = 0, const int offsetX = 0, const int offsetY = 0, const bool borders = true) const;
	void print(const GameField* field, const int offsetX = 0, const int offsetY = 0) const;
	void printBorders(const GameField* field, const bool animate = false, const int offsetX = 0, const int offsetY = 0) const;
	void printPreview();

	bool handleTetrominoLanding(const bool collided);
	void clearLinesAndUpdateScore();
	bool checkLineState(const GameField* field, const size_t lineNumber, const bool checkForFull = true) const;
	void shiftLines(GameField* field, const size_t maxPasses = 4, const bool checkType = true);
	int clearAndShiftLines(GameField* field);

	void endMessage() const;
	bool GameOver(const GameField* field, const Tetromino* tetro) const;
	void GameOverAnimation() const;

	void gameControls(const int userInput);

private:
	int level;
	int score;
	int totalClearedLines = 0;
	int userInput;
	GameField field;
	GameField preview;
	Tetromino tetromino;
	Tetromino nextTetromino;
};
