#include "game.h"
#include <iostream>

#include <Windows.h>
int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	Game game;
	game.runGame();
	return EXIT_SUCCESS;
}