#include "game.h"
#include "data.h"
#include "rng.h"

extern Interface UI;

#include <cstdio>

Game::Game() {
	BEING = being; // makes list of beings globally available
	QUEUE = &queue; // same for event queue
	
	map = new Map();
}

Game::~Game() {
	if (map)
		delete map;
}

void Game::init() {
    level = 1;
    being[0].createpc();
}

void Game::makelevel() {
	int x = -1, y, i, j;
	int numenemies, firstfree = 1;
	bool playerplaced = false;

	numenemies = 4 + rng(5) + level / 4;
	
	queue.empty();
	
	switch (level) {
		case 1:
			map->background(TILE_GRASS, TILE_GRASS);
			map->spots(TILE_TREE, 30);
			map->spots(TILE_EXIT, 1);
			break;
		case 2:
			map->background(TILE_GRASS, TILE_GRASS);
			map->spots(TILE_TREE, 120);
			map->spots(TILE_EXIT, 1);
			break;
		case 3:
			map->background(TILE_DIRT, TILE_DIRT);
			map->spots(TILE_ROUGH, 4);
			map->amoeba(TILE_ROUGH, 160);
			map->spots(TILE_ROCK, 80);
			map->spots(TILE_EXIT, 1);
			break;
		case 4:
			map->background(TILE_DIRT, TILE_ROUGH);
			map->amoeba(TILE_ROUGH, 720);
			map->spots(TILE_ROCK, 50);
			map->spots(TILE_EXIT, 1);
			break;
		case 5:
			map->background(TILE_ROUGH, TILE_ROUGH);
			map->spots(TILE_TREE, 16);
			x = 17; y = 26;
			map->path(TILE_ROAD, x, y, 7);
			x = rng(MAPHEIGHT); y = 0;
			being[0].place(map, x, y);
			playerplaced = true;
			map->rect(2, 16, 12, 40, TILE_FLOOR, TILE_WALL);
			map->walls(TILE_WALL, 2, 16, 12, 40, 20, 0);
			map->find(TILE_FLOOR, x, y);
			map->settile(x, y, TILE_EXIT);
			map->settile(16, 26, TILE_FLOOR);
			break;
		case 6:
			map->building(TILE_FLOOR, TILE_WALL, 28, 100);
			map->find(TILE_FLOOR, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 7:
			map->background(TILE_FLOOR, TILE_WALL);
			map->vault(x, y);
			for (i = 2; i < x; i++)
				map->settile(i, y, TILE_WALL);
			for (i = 2; i < y; i++)
				map->settile(x+4, i, TILE_WALL);
			for (i = x + 5; i < MAPHEIGHT - 2; i++)
				map->settile(i, y+5, TILE_WALL);
			for (i = y + 6; i < MAPWIDTH - 2; i++)
				map->settile(x, i, TILE_WALL);
			map->walls(TILE_WALL, 0, x+4, 0, y, 24, 20);
			map->walls(TILE_WALL, 0, x, y, MAPWIDTH-1, 24, 20);
			map->walls(TILE_WALL, x+4, MAPHEIGHT-1, 0, y+5, 24, 20);
			map->walls(TILE_WALL, x, MAPHEIGHT-1, y+5, MAPWIDTH-1, 24, 20);
			for (i = x + 1; i < x + 4; i++)
				for (j = y + 1; j < y + 5; j++) {
					being[firstfree] = Being(selectbeing(level), firstfree);
					being[firstfree].place(map, i, j);
					firstfree++;
				}
			numenemies += 12;
			map->find(TILE_FLOOR, x, y);
			being[0].place(map, x, y);
			playerplaced = true;
			map->find(TILE_FLOOR, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 8:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_CAVEFLOOR, x, y, 16);
			map->rooms(TILE_CAVEFLOOR, TILE_CAVEFLOOR, 4 + rng(4));
			map->settile(x, y, TILE_EXIT);
			break;
		case 9:
            {
                int playerx, playery;
                do {
                    map->background(TILE_DIRT, TILE_DIRT);
                    map->spots(TILE_ROUGH, 12);
                    map->amoeba(TILE_ROUGH, 300);
                    map->chasm();
                    x = rng(MAPWIDTH / 2 - 3);
                    y = rng(MAPHEIGHT);
                    playerx = x;
                    playery = y;
                    map->path(TILE_ROAD, x, y, 8);
                } while (y <= MAPWIDTH / 2 + 3);
                being[0].place(map, playerx, playery);
                playerplaced = true;
                map->settile(x, y, TILE_EXIT);
            }
			break;
		case 10:
			map->background(TILE_ROCK, TILE_ROCK);
			map->rect(0, MAPHEIGHT / 2, MAPWIDTH / 2, MAPWIDTH - 1,
							TILE_DIRT, TILE_DIRT);
			map->amoeba(TILE_DIRT, 600);
			map->rect(0, MAPHEIGHT / 3, MAPWIDTH * 2 / 3, MAPWIDTH - 1,
							TILE_SHALLOW, TILE_SHALLOW);
			map->rooms(TILE_SHALLOW, TILE_SHALLOW, 4);
			map->amoeba(TILE_SHALLOW, 100);
			map->find(TILE_DIRT, x, y);
			being[0].place(map, x, y);
			playerplaced = true;
			map->find(TILE_DIRT, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 11:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_CAVEFLOOR, x, y, 12);
			being[0].place(map, x, y);
			playerplaced = true;
			map->amoeba(TILE_CAVEFLOOR, 120);
			map->spots(TILE_SHALLOW, 4);
			map->amoeba(TILE_SHALLOW, 120);
			map->find(TILE_CAVEFLOOR, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 12:
			map->background(TILE_DEEP, TILE_SHALLOW);
			map->amoeba(TILE_SHALLOW, 640);
			map->find(TILE_SHALLOW, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 13:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->rect(0, MAPHEIGHT - 1, 0, MAPWIDTH / 3, TILE_DEEP, TILE_DEEP);
			map->amoeba(TILE_CAVEWALL, 24);
			map->path(TILE_SHALLOW, x, y, 20);
			map->amoeba(TILE_SHALLOW, 240);
			map->find(TILE_SHALLOW, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 14:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_DARK, x, y, 18);
			map->rooms(TILE_DARK, TILE_CAVEFLOOR, rng(5) + 4);
			map->settile(x, y, TILE_EXIT);
			break;
		case 15:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_DARK, x, y, 14);
			map->rooms(TILE_DARK, TILE_CAVEFLOOR, rng(3) + 2);
			map->vault(x, y); // may be disconnected; intentional
			for (i = x + 1; i < x + 4; i++)
				for (j = y + 1; j < y + 5; j++) {
					being[firstfree] = Being(selectbeing(level), firstfree);
					being[firstfree].place(map, i, j);
					firstfree++;
				}
			numenemies += 12;
			map->find(TILE_DARK, x, y);
			map->settile(x, y, TILE_EXIT);
			being[0].place(map, x, y);
			playerplaced = true;
			break;
		case 16:
			map->background(TILE_CHASM, TILE_CHASM);
			map->spots(TILE_CAVEFLOOR, 1);
			map->rooms(TILE_CAVEFLOOR, TILE_CAVEFLOOR, 4);
			map->amoeba(TILE_CAVEFLOOR, 32);
			for (i = 8 + rng(5); i > 0; i--) {
				map->find(TILE_CAVEFLOOR, x, y);
				map->path(TILE_CAVEFLOOR, x, y, 2 + rng(3));
			}
			map->settile(x, y, TILE_EXIT);
			break;
		case 17:
			map->building(TILE_SHALLOW, TILE_RUIN, 26, 30);
			map->amoeba(TILE_SHALLOW, 100);
			map->find(TILE_SHALLOW, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 18:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->rect(MAPHEIGHT/2-1, MAPHEIGHT/2+2, MAPWIDTH/2-3,
							MAPWIDTH/2+4, TILE_SUB, TILE_SUB);
			map->amoeba(TILE_SUB, 600);
			map->find(TILE_SUB, x, y);
			being[0].place(map, x, y);
			playerplaced = true;
			map->settile(x, y, TILE_EXIT);
			break;
		case 19:
			map->building(TILE_FLOOR, TILE_WALL, 0, 40);
			map->find(TILE_FLOOR, x, y);
			map->settile(x, y, TILE_EXIT);
			break;
		case 20:
			map->building(TILE_METALFLOOR, TILE_METALWALL, 40, 0);
			map->find(TILE_METALFLOOR, x, y);
			map->settile(x, y, TILE_EXIT);
			numenemies = MAXNUMBEINGS - 1;
			break;
	}
	
	being[0].type = BEING_PLAYER;
	for (i = 0; i < EQSIZE; i++)
		being[0].eq[i].status = 100;
	if (!playerplaced)
		do {
        } while (!being[0].placerandom(map));
	
	if (numenemies + 1 > MAXNUMBEINGS)
		numenemies = MAXNUMBEINGS - 1;
	for (i = firstfree; i < numenemies + 1; i++) {
		being[i] = Being(selectbeing(level), i);
		if (!being[i].placerandom(map))
			being[i] = Being();
	}
    startplay();
}

void Game::normaldisplay() {
    UI.clearscreen();
    map->displayall();
    being[0].displaystatus();
}

void Game::startplay() {	
	prevtarget = 0;
    normaldisplay();
    state = GAMESTATE_RUNNING;
	being[0].hasmoved();
}

void Game::mainloop() {
	int time;
	Event event;
	char msgbuf[128];

	makelevel();
	
	do {
		do {
			event = queue.pop();
			switch (event.type) {
				case EVENT_ACTION:
					do {
						time = being[event.being].act();
					} while (time == 0 && state == GAMESTATE_RUNNING); // FIXME
					if (being[event.being].type != BEING_VOID
                            && being[event.being].type != BEING_WRECK) {
						event.time = time;
						queue.push(event);
					}
					if (being[0].type == BEING_VOID
                            || being[0].type == BEING_WRECK)
                        state = GAMESTATE_YASD;
					break;

				case EVENT_ENERGY:
					event.time = being[event.being].energycycle();
					if (event.time)
						queue.push(event);
					break;
				case EVENT_SELFENERGY:
					int tmp = event.being;
					if (tmp < 0 || tmp >= MAXNUMBEINGS)
						UI.msg("Assert Error!");
					event.time = being[event.being].selfenergycycle();
					if (event.time)
						queue.push(event);
					break;
			}
		} while (state == GAMESTATE_RUNNING);

		if (state == GAMESTATE_NEXTLEVEL) {
			// player reached level exit
			level++;
			if (level <= 20) {
				strcpy(msgbuf, "You go to level ");
				intcat(msgbuf, level);
				strcat(msgbuf, ".");
				UI.msg(msgbuf);
				makelevel();
				state = GAMESTATE_RUNNING;
			}
            else
                state = GAMESTATE_YAVP;
		}
	} while (state == GAMESTATE_RUNNING);

	if (state != GAMESTATE_YAVP) {
		// loss or quit
		strcpy(msgbuf, "You managed to reach level ");
		intcat(msgbuf, level);
		strcat(msgbuf, " this game. Better luck next time!");
		UI.msg(msgbuf);
	}
	else {
		// win
		UI.msg("Congratulations! You have completed all 20 levels of the game! There's probably a great reward for you and your robot at the bottom of the dungeon, but I'm afraid I haven't implemented it... Anyway, thank you for your interest in this game.");
	}
}

bool Game::savegame() {
    std::ofstream fout;

    fout.exceptions(std::ofstream::eofbit
                    | std::ofstream::failbit
                    | std::ofstream::badbit);
    try {
        fout.open("scrap.sav");
        save(fout);
    }
    catch (std::ofstream::failure e) {
        return false;
    }
    fout.close();

    return true;
}

bool Game::loadgame() {
    std:: ifstream fin;

    fin.exceptions(std::ifstream::eofbit
                   | std::ifstream::failbit
                   | std::ifstream::badbit);
    try {
        fin.open("scrap.sav");
        load(fin);
    }
    catch (std::ofstream::failure e) {
        return false;
    }
    catch (DataException e) {
        return false;
    }
    fin.close();
    remove("scrap.sav");
    startplay();

    return true;
}

void Game::save(std::ostream & out) {
    out << SAVEFILEVERSION << '\n'
        << level << '\n';
    for (int i = 0; i < MAXNUMBEINGS; i++)
        being[i].save(out);
    map->save(out);
    queue.save(out);
}

void Game::load(std::istream & in) {
    int version;

    in >> version;
    if (version != SAVEFILEVERSION) {
        UI.msg("This file's format is incompatible with this version of the game.");
        throw DataException();
    }
    in >> level;
    for (int i = 0; i < MAXNUMBEINGS; i++)
        being[i].load(in);
    map->load(in);
    queue.load(in);
}
