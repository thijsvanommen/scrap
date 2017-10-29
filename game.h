/*
 * Scrap, the Roguelike
 * Copyright 2005, 2006 Thijs van Ommen
 *
 * This file is part of Scrap, the Roguelike.
 *
 * Scrap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Scrap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scrap.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GAME_H
#define GAME_H

class Game;

const char SCRAP_VERSION[] = "Scrap, the Extended 10DRL 1.1";
const int SAVEFILEVERSION = 1;
const char PREVVERSION_0[] = "Scrap, the Extended 10DRL";
const char * const SAVEFILEVERSIONSUPPORT[SAVEFILEVERSION] = {
    PREVVERSION_0
};

enum Gamestate {
    GAMESTATE_RUNNING, GAMESTATE_NEXTLEVEL,
    GAMESTATE_YASD, GAMESTATE_YAVP,
    GAMESTATE_SAVE, GAMESTATE_LOAD, GAMESTATE_QUIT
};

#include <fstream>
#include <vector>

#include "event.h"
#include "being.h"
#include "map.h"

class Game {
public:
    Game();
    ~Game();
    Game & operator=(const Game & rhs);

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
    std::vector<Being> being;
    Map * map;
    EventQueue queue;
    int level;
    int prevtarget;
};

extern Game GAME;

#endif
