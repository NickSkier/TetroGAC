#pragma once

#include "GameField.h"
#include "Tetromino.h"

class Game {
public:
	bool GameOver(GameField* field, Tetromino* tetro);
};
