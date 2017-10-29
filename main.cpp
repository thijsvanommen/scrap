#include "main.h"

int main() {
	if (!UI.init())
		return 1;

	seedrng();

	Game * game = new Game;

	UI.msg("Welcome to Scrap, the Ten-Day Roguelike! Press '?' for help.");
	game->mainloop();

	delete game;
	
	UI.close();

	return 0;
}

