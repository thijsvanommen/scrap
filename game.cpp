#include "game.h"

Game::Game() {
	BEING = being; // makes list of beings globally available
	QUEUE = &queue; // same for event queue
	
	map = NULL;
	level = 1;

	being[0].createpc();
}

Game::~Game() {
	if (map)
		delete map;
}

void Game::makelevel() {
	int x = -1, y, i, j;
	int numenemies, firstfree = 1;
	bool playerplaced = false;
	
	map = new Map();
	
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
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			*map->gettileaddr(16, 26) = Tile(TILE_FLOOR);
			break;
		case 6:
			map->building(TILE_FLOOR, TILE_WALL, 28, 100);
			map->find(TILE_FLOOR, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 7:
			map->background(TILE_FLOOR, TILE_WALL);
			map->vault(x, y);
			for (i = 2; i < x; i++)
				*map->gettileaddr(i, y) = Tile(TILE_WALL);
			for (i = 2; i < y; i++)
				*map->gettileaddr(x+4, i) = Tile(TILE_WALL);
			for (i = x + 5; i < MAPHEIGHT - 2; i++)
				*map->gettileaddr(i, y+5) = Tile(TILE_WALL);
			for (i = y + 6; i < MAPWIDTH - 2; i++)
				*map->gettileaddr(x, i) = Tile(TILE_WALL);
			map->walls(TILE_WALL, 0, x+4, 0, y, 24, 20);
			map->walls(TILE_WALL, 0, x, y, MAPWIDTH-1, 24, 20);
			map->walls(TILE_WALL, x+4, MAPHEIGHT-1, 0, y+5, 24, 20);
			map->walls(TILE_WALL, x, MAPHEIGHT-1, y+5, MAPWIDTH-1, 24, 20);
			for (i = x + 1; i < x + 4; i++)
				for (j = y + 1; j < y + 5; j++) {
					being[firstfree] = Being(selectbeing(level));
					being[firstfree].place(map, i, j, firstfree);
					firstfree++;
				}
			numenemies += 12;
			map->find(TILE_FLOOR, x, y);
			being[0].place(map, x, y);
			playerplaced = true;
			map->find(TILE_FLOOR, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 8:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_CAVEFLOOR, x, y, 16);
			map->rooms(TILE_CAVEFLOOR, TILE_CAVEFLOOR, 4 + rng(4));
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 9:
			do {
				map->background(TILE_DIRT, TILE_DIRT);
				map->spots(TILE_ROUGH, 12);
				map->amoeba(TILE_ROUGH, 300);
				map->chasm();
				x = rng(MAPWIDTH / 2 - 3);
				y = rng(MAPHEIGHT);
				being[0].place(map, x, y);
				map->path(TILE_ROAD, x, y, 8);
			} while (y <= MAPWIDTH / 2 + 3);
			playerplaced = true;
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
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
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
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
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 12:
			map->background(TILE_DEEP, TILE_SHALLOW);
			map->amoeba(TILE_SHALLOW, 640);
			map->find(TILE_SHALLOW, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 13:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->rect(0, MAPHEIGHT - 1, 0, MAPWIDTH / 3, TILE_DEEP, TILE_DEEP);
			map->amoeba(TILE_CAVEWALL, 24);
			map->path(TILE_SHALLOW, x, y, 20);
			map->amoeba(TILE_SHALLOW, 240);
			map->find(TILE_SHALLOW, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 14:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_DARK, x, y, 18);
			map->rooms(TILE_DARK, TILE_CAVEFLOOR, rng(5) + 4);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 15:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->path(TILE_DARK, x, y, 14);
			map->rooms(TILE_DARK, TILE_CAVEFLOOR, rng(3) + 2);
			map->vault(x, y); // may be disconnected; intentional
			for (i = x + 1; i < x + 4; i++)
				for (j = y + 1; j < y + 5; j++) {
					being[firstfree] = Being(selectbeing(level));
					being[firstfree].place(map, i, j, firstfree);
					firstfree++;
				}
			numenemies += 12;
			map->find(TILE_DARK, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
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
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 17:
			map->building(TILE_SHALLOW, TILE_RUIN, 26, 30);
			map->amoeba(TILE_SHALLOW, 100);
			map->find(TILE_SHALLOW, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 18:
			map->background(TILE_CAVEWALL, TILE_CAVEWALL);
			map->rect(MAPHEIGHT/2-1, MAPHEIGHT/2+2, MAPWIDTH/2-3,
							MAPWIDTH/2+4, TILE_SUB, TILE_SUB);
			map->amoeba(TILE_SUB, 600);
			map->find(TILE_SUB, x, y);
			being[0].place(map, x, y);
			playerplaced = true;
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 19:
			map->building(TILE_FLOOR, TILE_WALL, 0, 40);
			map->find(TILE_FLOOR, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			break;
		case 20:
			map->building(TILE_METALFLOOR, TILE_METALWALL, 40, 0);
			map->find(TILE_METALFLOOR, x, y);
			*map->gettileaddr(x, y) = Tile(TILE_EXIT);
			numenemies = MAXNUMBEINGS - 1;
			break;
	}
	
	being[0].type = BEING_PLAYER;
	for (i = 0; i < EQSIZE; i++)
		being[0].eq[i].status = 100;
	if (!playerplaced)
		being[0].placerandom(map);
	
	if (numenemies + 1 > MAXNUMBEINGS)
		numenemies = MAXNUMBEINGS - 1;
	for (i = firstfree; i < numenemies + 1; i++) {
		being[i] = Being(selectbeing(level));
		if (!being[i].placerandom(map, i))
			being[i] = Being();
	}
	
	prevtarget = 0;

	map->displayall();
	being[0].hasmoved();
	being[0].displaystatus();
}

void Game::mainloop() {
	int time;
	Event event;
	bool running = true;
	char msgbuf[128];

	makelevel();
	
	do {
		do {
			event = queue.pop();
			switch (event.type) {
				case EVENT_ACTION:
					do {
						time = being[event.being].act();
					} while (time == 0);
					if (being[event.being].type != BEING_VOID
									&& being[event.being].type != BEING_WRECK
									&& being[event.being].type
									!= BEING_PLAYERDESCENDED) {
						event.time = time;
						queue.push(event);
					}

					if (being[0].type == BEING_VOID
									|| being[0].type == BEING_WRECK
									|| being[0].type == BEING_PLAYERDESCENDED)
						running = false;
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
		} while (running);

		if (being[0].type == BEING_PLAYERDESCENDED) {
			// player reached level exit
			level++;
			if (level <= 20) {
				strcpy(msgbuf, "You go to level ");
				intcat(msgbuf, level);
				strcat(msgbuf, ".");
				UI.msg(msgbuf);
				makelevel();
				running = true;
			}
		}
	} while (running);

	if (level <= 20) {
		// loss
		strcpy(msgbuf, "You managed to reach level ");
		intcat(msgbuf, level);
		strcat(msgbuf, " this game. Better luck next time!");
		UI.msg(msgbuf);
	}
	else {
		// win
		UI.msg("Congratulations! You have completed all 20 levels of the game! There's probably a great reward for you and your robot at the bottom of the dungeon, but I'm afraid I haven't implemented it... Anyway, thank you for your interest in this game.");
	}

	while (getch() == ERR);
}
