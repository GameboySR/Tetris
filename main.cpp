#include <cstring>
#include "SDL.h"
#include "game.h"
#include <ctime>

int main(int argc, char* args[]) {
	(void)argc;
	(void)args;
	srand(time(nullptr));
	Game game(350); //game takes the width of the tetris grid, height is set to 2*width
	game.run();

	return 0;
}