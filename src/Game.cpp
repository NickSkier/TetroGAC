#include <string>
#include <ncurses.h>
#include "Game.h"
#include "GameField.h"
#include "Tetromino.h"

bool Game::GameOver(GameField* field, Tetromino* tetro) {
	std::string gameOverMessage = "Game Over!";
	int userInterrupt;
	bool shouldBrake = false;

	if (tetro->checkCollisions(field) && tetro->getY() > F_VISIBLE_HEIGHT) { // The whole logic of the Game Over
		// Everything lower is just a message and an animation of exiting
		attron(A_BOLD);
		mvprintw(F_VISIBLE_HEIGHT / 2, F_WIDTH, "%s", gameOverMessage.c_str());
		refresh();
		attroff(A_BOLD);
		getchar();
		for (size_t i = 0; i < F_HEIGHT + 5; ++i) {
			for (size_t j = 0; j < F_WIDTH * 2 + 15; ++j) {
				userInterrupt = getch();
				if (userInterrupt != ERR) {
					shouldBrake = true;
					break;
				}
				else {
					if (!(i == F_VISIBLE_HEIGHT / 2 && j >= F_WIDTH && j < F_WIDTH + gameOverMessage.length())) {
						mvprintw(i, j, " ");
						refresh();
						napms(2);
					}
				}
			}
			if (shouldBrake) break;
		}
		return true;
	}
	return false;
}
