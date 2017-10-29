#ifndef GAME_H
#define GAME_H

class Game;

#include "being.h"
#include "map.h"
#include "event.h"

class Game {
public:
	Game();
	~Game();

	void makelevel();
	void mainloop();

	Being being[MAXNUMBEINGS];
	Map * map;
	EventQueue queue;
	int level;
};

#endif
