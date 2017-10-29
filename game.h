#ifndef GAME_H
#define GAME_H

class Game;

const char SCRAP_VERSION [] = "Scrap, the Extended 10DRL";
const int SAVEFILEVERSION = 0;
const int MAXNUMBEINGS = 24;

enum Gamestate {
    GAMESTATE_RUNNING, GAMESTATE_NEXTLEVEL,
    GAMESTATE_YASD, GAMESTATE_YAVP, GAMESTATE_QUIT
};

#include <fstream>

#include "being.h"
#include "map.h"
#include "event.h"

class Game {
public:
	Game();
	~Game();

    void init();
	void makelevel();
    void startplay();
    void normaldisplay();
	void mainloop();

    bool savegame();
    bool loadgame();

    void save(std::ostream & out);
    void load(std::istream & in);

    Gamestate state;
	Being being[MAXNUMBEINGS];
	Map * map;
	EventQueue queue;
	int level;
};

extern Game GAME;

#endif
