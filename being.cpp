#include "being.h"
#include "game.h"
#include "event.h"
#include "rng.h"

extern Interface UI;

Being::Being():
type(BEING_VOID), isplayer(false), repairing(-1), mobile(true), id(0) {}

Being::Being(BeingType newtype, int newid):
type(newtype), isplayer(false), repairing(-1), id(newid),
thinksplayeristhere(false), timesincelasthit(99) {
	mobile = true;
	switch(newtype) {
		case BEING_BUGGY: // 0
			habitat = TP_TRACK;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("minigun", 12, DAMAGE_NORMAL, 1, TW_NORMAL);
			eq[2] = Propulsion("buggy drive", 9, TP_LAND);
			eq[3] = Sensor("mini-range sensor", 2, TS_NORMAL);
			break;
		case BEING_PROBE: // 1
			habitat = TP_AIR | TERRAIN_TUNNEL | TERRAIN_DARK;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("weak laser gun", 15, DAMAGE_SENSOR, 2, TW_NORMAL);
			eq[2] = Propulsion("hovering thruster", 0, habitat);
			eq[3] = Sensor("scanner", 4, TS_AIR);
			break;
		case BEING_SENTRY: // 2
			habitat = TP_TRACK;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("disrupter", 18, DAMAGE_WEAPON, 1, TW_NORMAL);
			eq[2] = Sensor("X-ray scope", 2, TS_XRAY);
			break;
		case BEING_HUNTER: // 3
			habitat = TP_DEF;
			eq[0] = Power("power core", 8);
			eq[1] = Power("emergency power", 32, 20);
			eq[2] = Weapon("phaser rifle", 18, DAMAGE_POWER, 2, TW_NORMAL);
			eq[3] = Propulsion("two-wheel drive", 6, TP_LAND);
			eq[4] = Sensor("short-range sensor", 3, TS_NORMAL);
			break;
		case BEING_TANK: // 4
			habitat = TP_TRACK;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("gun", 20, DAMAGE_NORMAL, 2, TW_NORMAL);
			eq[2] = Propulsion("caterpillar track", 3, habitat);
			eq[3] = Sensor("short-range sensor", 3, TS_NORMAL);
			eq[4] = Armor("tank armor", 15, 0);
			break;
		case BEING_AMBUSHER: // 5
			habitat = TP_DEF;
			eq[0] = Power("power core", 8);
			eq[1] = Weapon("taser", 30, DAMAGE_POWER, 1, TW_NORMAL);
			eq[2] = Weapon("bazooka", 40, DAMAGE_ARMOR, 4, TW_AIR, 30);
			eq[3] = Propulsion("caterpillar track", 3, TP_TRACK);
			eq[4] = Sensor("scanner", 4, TS_AIR);
			break;
		case BEING_SECURITY: // 6
			habitat = TP_TRACK | TERRAIN_HIGH;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("automatic gun", 32, DAMAGE_NORMAL, 3, TW_AIR);
			eq[2] = Sensor("security camera", 4, TS_INFRA);
			mobile = false;
			break;
		case BEING_CYBORG: // 7
			habitat = TP_DEF;
			eq[0] = Power("biocore", 12);
			eq[1] = Power("emergency power", 32, 20);
			eq[2] = Weapon("flamethrower", 30, DAMAGE_ARMOR, 3, TW_NORMAL & ~TW_WATER);
			eq[3] = Propulsion("cybernetic leg", 3, TP_LAND);
			eq[4] = Propulsion("jetpack", 15, TP_AIR, 8);
			eq[5] = Sensor("X-ray visor", 3, TS_XRAY);
			eq[6] = Armor("cybernetic armor", 16, DAMAGE_NORMAL);
			break;
		case BEING_JUGGERNAUT: // 8
			habitat = TP_DEF;
			eq[0] = Power("steam engine", 0);
			eq[1] = Weapon("grinder", 45, DAMAGE_NORMAL, 1, TW_NORMAL);
			eq[2] = Propulsion("spiked wheel", 1, TP_TRACK);
			eq[3] = Sensor("motion detector", 4, TS_NORMAL);
			eq[4] = Armor("front plate", 24, DAMAGE_WEAPON);
			eq[5] = Armor("aft plate", 24, DAMAGE_POWER);
			break;
		case BEING_ARMADILLO: // 9
			habitat = TP_DEF | TP_ROCK;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("sonic gun", 40, DAMAGE_SENSOR, 2, TW_NORMAL | TW_TREMOR);
			eq[2] = Propulsion("burrowing claw", 0, TP_LAND | TP_ROCK);
			eq[3] = Sensor("armadillo visor", 2, TS_XRAY | TERRAIN_DARK);
			eq[4] = Armor("armadillo plate", 20, DAMAGE_PROPULSION);
			break;
		case BEING_PLANT: // 10
			habitat = TP_ROCK;
			eq[0] = Power("reactor core", 12);
			eq[1] = Power("reactor core", 12);
			eq[2] = Power("reactor core", 12);
			eq[3] = Power("reactor core", 12);
			mobile = false;
			break;
		case BEING_AMPHIBIAN: // 11
			habitat = TP_DEF | TERRAIN_DEEP;
			eq[0] = Power("power core", 8);
			eq[1] = Weapon("heavy gun", 45, DAMAGE_NORMAL, 3, TW_NORMAL);
			eq[2] = Propulsion("four-wheel drive", 7, TP_TRACK);
			eq[3] = Propulsion("floater", 2, TP_WATER);
			eq[4] = Sensor("motion detector", 4, TS_NORMAL);
			eq[5] = Armor("iron plating", 10, 0);
			break;
		case BEING_DESTROYER: // 12
			habitat = TP_WATER;
			eq[0] = Power("power core", 8);
			eq[1] = Weapon("extended gun", 35, DAMAGE_NORMAL, 4, TW_NORMAL);
			eq[2] = Weapon("torpedo tube", 45, DAMAGE_PROPULSION, 3, TW_WATER);
			eq[3] = Propulsion("propeller", 6, TP_WATER);
			eq[4] = Sensor("long-range sensor", 5, TS_NORMAL);
			eq[5] = Sensor("sonar pulse emitter", 9, TS_SONAR, 20);
			eq[6] = Armor("hull plating", 24, DAMAGE_PROPULSION);
			break;
		case BEING_MINE: // 13
			habitat = TP_DEF | TP_WATER;
			eq[0] = Weapon("detonator", 55, DAMAGE_NORMAL, 3, TW_NORMAL, 100);
			mobile = false;
			break;
		case BEING_BATTLESHIP: // 14
			habitat = TP_WATER;
			eq[0] = Power("heavy combustor", 14);
			eq[1] = Weapon("extended heavy gun", 45, DAMAGE_NORMAL, 4, TW_NORMAL);
			eq[2] = Propulsion("heavy-duty propeller", 3, TP_WATER);
			eq[3] = Sensor("long-range sensor", 5, TS_NORMAL);
			eq[4] = Sensor("short-range radar", 2, TS_INFRA | TERRAIN_HIGH);
			eq[5] = Armor("enhanced hull plating", 32, DAMAGE_PROPULSION);
			eq[6] = Armor("interior hull plating", 24, DAMAGE_PROPULSION);
			break;
		case BEING_GEOLOGIST: // 15
			habitat = TP_ROCK;
			eq[0] = Power("power core", 8);
			eq[1] = Weapon("quake engine", 55, DAMAGE_NORMAL, 3, TW_TREMOR, 40);
			eq[2] = Propulsion("magnetic propulsion", 0, TP_ROCK);
			eq[3] = Sensor("seismic sensor", 7, TS_TREMOR);
			eq[4] = Armor("mineral shield", 20, DAMAGE_NORMAL | DAMAGE_ARMOR, 1);
			break;
		case BEING_BOMBER: // 16
			habitat = TP_AIR;
			eq[0] = Power("combusion engine", 4);
			eq[1] = Weapon("bomb array", 45, DAMAGE_NORMAL, 2, TW_AIR);
			eq[2] = Propulsion("jet drive", 12, TP_AIR);
			eq[3] = Sensor("mid-range radar", 5, TS_INFRA | TERRAIN_HIGH);
			eq[4] = Armor("energy shield", 35, DAMAGE_POWER, 4);
			break;
		case BEING_SUBMARINE: // 17
			habitat = TP_SUB;
			eq[0] = Power("nuclear core", 16, 2);
			eq[1] = Power("nuclear core", 16, 2);
			eq[2] = Weapon("heavy torpedo tube", 55, DAMAGE_PROPULSION, 8, TW_WATER, 10);
			eq[3] = Propulsion("submarine drive", 8, TP_SUB);
			eq[4] = Sensor("sonar", 9, TS_SONAR);
			eq[5] = Armor("fortified hull", 32, DAMAGE_PROPULSION | DAMAGE_ARMOR);
			eq[6] = Armor("improved energy shield", 50, DAMAGE_POWER, 5);
			break;
		case BEING_DRACO: // 18
			habitat = TP_DEF | TP_AIR;
			eq[0] = Power("dragonheart", 24, 3);
			eq[1] = Weapon("heavy flamethrower", 60, DAMAGE_ARMOR, 5, TW_AIR & ~TW_WATER, 10);
			eq[2] = Weapon("shredder", 75, DAMAGE_PROPULSION, 1, TW_AIR & ~TERRAIN_CHASM & ~TERRAIN_HIGH, 12);
			eq[3] = Propulsion("gas balloon", 10, TP_AIR);
			eq[4] = Sensor("dragoneye", 6, TS_AIR | TERRAIN_DARK);
			eq[5] = Armor("scale armor", 40, 0);
			break;
		case BEING_LAUNCHER: // 19
			habitat = TP_DEF | TERRAIN_HIGH;
			eq[0] = Power("power cell", 0);
			eq[1] = Weapon("long-range artillery", 60, DAMAGE_NORMAL, 16, TW_AIR, 75);
			eq[2] = Sensor("long-range radar", 16, TS_INFRA | TERRAIN_HIGH, 10);
			eq[3] = Armor("front shield", 40, DAMAGE_WEAPON);
			mobile = false;
			break;
		case BEING_STEELELEMENTAL: // 20
			habitat = TP_DEF;
			eq[0] = Power("heart of steel", 24);
			eq[1] = Weapon("steelshredder", 72, DAMAGE_NORMAL, 1, TW_NORMAL);
			eq[2] = Propulsion("steel leg", 12, TP_TRACK);
			eq[3] = Propulsion("quicksilver leg", 16, TP_TRACK, 25);
			eq[4] = Sensor("magnetic field detector", 7, TS_NORMAL | TERRAIN_METAL);
			eq[5] = Armor("organic steel plate", 40, DAMAGE_ARMOR);
			break;
		case BEING_MECHALICH: // 21
			habitat = TP_DEF;
			eq[0] = Power("power core of regeneration", 32);
			eq[1] = Weapon("fireball", 72, DAMAGE_ARMOR, 6, TW_NORMAL, 30);
			eq[2] = Weapon("cold ray", 72, DAMAGE_PROPULSION, 6, TW_NORMAL, 30);
			eq[3] = Weapon("lightning bolt", 72, DAMAGE_SENSOR, 6, TW_NORMAL, 30);
			eq[4] = Propulsion("mecha leg", 9, TP_TRACK);
			eq[5] = Sensor("orb of scrying", 12, TS_NORMAL | TS_TREMOR);
			eq[6] = Armor("advanced techno-armor", 30, DAMAGE_POWER | DAMAGE_WEAPON | DAMAGE_PROPULSION | DAMAGE_SENSOR | DAMAGE_ARMOR);
			eq[7] = Armor("invulnerability sphere", 999, 0, 30);
			break;
    case BEING_VOID:
    case BEING_WRECK:
    case BEING_PLAYER:
        habitat = 0;
        eq[0] = Power("buggy being power", 0);
        break;
	}
}

void Being::createpc() {
	type = BEING_PLAYER;
	isplayer = true;
	habitat = TP_DEF;
	eq[0] = Power("power cell", 0);
	eq[1] = Weapon("medium gun", 35, DAMAGE_NORMAL, 2, TW_NORMAL);
	eq[2] = Propulsion("two-wheel drive", 6, TP_LAND);
	eq[3] = Sensor("short-range sensor", 3, TS_NORMAL);
	//eq[3] = Sensor("sightseer", 16, 0xffff);
	eq[4] = Armor("iron plating", 10, 0);
    eq[5] = Item();
    eq[6] = Item();
    eq[7] = Item();
}

void Being::place(Map * newmap, int newx, int newy) {
	map = newmap;
	xpos = newx;
	ypos = newy;
	map->setbeing(xpos, ypos, this);
	QUEUE->push(Event(1, EVENT_ACTION, getid()));
	QUEUE->push(Event(0, EVENT_ENERGY, getid()));
	QUEUE->push(Event(0, EVENT_SELFENERGY, getid()));
}

bool Being::placerandom(Map * newmap) {
	int newx, newy;
	int tries = 0;
	bool ok = false;

	do {
		newx = rng(MAPHEIGHT);
		newy = rng(MAPWIDTH);
		tries++;
		if (newmap->gettile(newx, newy).getbeing() == NULL
						&& newmap->gettile(newx, newy).terraintype() & habitat)
			ok = true;
	} while (!ok && tries < 1000);
	
	if (ok)
		place(newmap, newx, newy);

	return ok;
}

void Being::getappearance(char & symbol, int & fore) {
	switch (type) {
		case BEING_WRECK:
			symbol = '%'; fore = COLOR_WHITE; break;
		case BEING_PLAYER:
			symbol = '@'; fore = COLOR_LWHITE; break;
		case BEING_BUGGY:
			symbol = 'b'; fore = COLOR_LYELLOW; break;
		case BEING_PROBE:
			symbol = 'p'; fore = COLOR_LBLUE; break;
		case BEING_SENTRY:
			symbol = 's'; fore = COLOR_LRED; break;
		case BEING_HUNTER:
			symbol = 'h'; fore = COLOR_LWHITE; break;
		case BEING_TANK:
			symbol = 'T'; fore = COLOR_GREEN; break;
		case BEING_AMBUSHER:
			symbol = 'x'; fore = COLOR_LMAGENTA; break;
		case BEING_SECURITY:
			symbol = 's'; fore = COLOR_LYELLOW; break;
		case BEING_CYBORG:
			symbol = 'b'; fore = COLOR_LCYAN; break;
		case BEING_JUGGERNAUT:
			symbol = 'H'; fore = COLOR_RED; break;
		case BEING_ARMADILLO:
			symbol = 'a'; fore = COLOR_YELLOW; break;
		case BEING_PLANT:
			symbol = 'p'; fore = COLOR_LBLUE; break;
		case BEING_AMPHIBIAN:
			symbol = 'F'; fore = COLOR_LGREEN; break;
		case BEING_DESTROYER:
			symbol = 's'; fore = COLOR_LYELLOW; break;
		case BEING_MINE:
			symbol = 'm'; fore = COLOR_LWHITE; break;
		case BEING_BATTLESHIP:
			symbol = 'S'; fore = COLOR_LYELLOW; break;
		case BEING_GEOLOGIST:
			symbol = 'G'; fore = COLOR_LMAGENTA; break;
		case BEING_BOMBER:
			symbol = 'p'; fore = COLOR_GREEN; break;
		case BEING_SUBMARINE:
			symbol = 'S'; fore = COLOR_WHITE; break;
		case BEING_DRACO:
			symbol = 'D'; fore = COLOR_RED; break;
		case BEING_LAUNCHER:
			symbol = 'M'; fore = COLOR_LYELLOW; break;
		case BEING_STEELELEMENTAL:
			symbol = 'E'; fore = COLOR_LCYAN; break;
		case BEING_MECHALICH:
			symbol = 'L'; fore = COLOR_LWHITE; break;
		default:
			symbol = 'E'; fore = COLOR_LRED; break;
	}
}

char * Being::getname() {
	switch (type) {
		case BEING_WRECK:
			return "wreck";
		case BEING_PLAYER:
			return "yourself";
		case BEING_BUGGY:
			return "buggy";
		case BEING_PROBE:
			return "probe";
		case BEING_SENTRY:
			return "sentry";
		case BEING_HUNTER:
			return "hunter";
		case BEING_TANK:
			return "tank";
		case BEING_AMBUSHER:
			return "ambusher";
		case BEING_SECURITY:
			return "security system";
		case BEING_CYBORG:
			return "cyborg";
		case BEING_JUGGERNAUT:
			return "juggernaut";
		case BEING_ARMADILLO:
			return "armadillo";
		case BEING_PLANT:
			return "power plant";
		case BEING_AMPHIBIAN:
			return "amphibian";
		case BEING_DESTROYER:
			return "destroyer";
		case BEING_MINE:
			return "mine";
		case BEING_BATTLESHIP:
			return "battleship";
		case BEING_GEOLOGIST:
			return "geologist";
		case BEING_BOMBER:
			return "bomber";
		case BEING_SUBMARINE:
			return "submarine";
		case BEING_DRACO:
			return "draco";
		case BEING_LAUNCHER:
			return "missile launcher";
		case BEING_STEELELEMENTAL:
			return "steel elemental";
		case BEING_MECHALICH:
			return "mechalich";
    case BEING_VOID:
        break;
	}
    return "buggy being";
}

int Being::getid() {
    return id;
}

int Being::act() {
	int time = 0;
	Command c;
	
	if (isplayer) {
		move(DISPLAYTOP + xpos, DISPLAYLEFT + ypos);
		c = UI.getcommand();
	}
	else {
		c = AI();
	}
	
	switch (c) {
		case COMMAND_NORTH:
			time = go(-1, 0); break;
		case COMMAND_NE:
			time = go(-1, 1); break;
		case COMMAND_EAST:
			time = go(0, 1); break;
		case COMMAND_SE:
			time = go(1, 1); break;
		case COMMAND_SOUTH:
			time = go(1, 0); break;
		case COMMAND_SW:
			time = go(1, -1); break;
		case COMMAND_WEST:
			time = go(0, -1); break;
		case COMMAND_NW:
			time = go(-1, -1); break;
		case COMMAND_WAIT:
			time = 80; break;
		case COMMAND_1:
			time = use(0); break;
		case COMMAND_2:
			time = use(1); break;
		case COMMAND_3:
			time = use(2); break;
		case COMMAND_4:
			time = use(3); break;
		case COMMAND_5:
			time = use(4); break;
		case COMMAND_6:
			time = use(5); break;
		case COMMAND_7:
			time = use(6); break;
		case COMMAND_8:
			time = use(7); break;
    case COMMAND_DETAILS:
        UI.detailspage(*this);
        GAME.normaldisplay();
        time = 0;
        break;
    case COMMAND_REORDER:
        time = reorderitems(); break;
		case COMMAND_SWITCH:
			time = onoff(); break;
		case COMMAND_DOWN:
			if (map->gettile(xpos, ypos).oftiletype(TILE_EXIT)) {
				GAME.state = GAMESTATE_NEXTLEVEL;
				time = 0;
			}
			else
				UI.msg("You need to find the exit first!");
			break;
		case COMMAND_HELP:
			UI.displayhelp(); map->displayall(); break;
    case COMMAND_SAVE:
        if (UI.confirm("Save this game and quit? (y/n)")) {
            if (GAME.savegame())
                GAME.state = GAMESTATE_QUIT;
            else
                UI.msg("An error occured while trying to write the savefile!");
            time = 0;
        }
        break;
    case COMMAND_LOAD:
        if (UI.confirm("Restore previous game? (y/n)")) {
            if (!GAME.loadgame())
                UI.msg("An error occured while trying to read the savefile!");
            time = 0;
        }
        break;
		case COMMAND_QUIT:
			if (UI.confirm(
                "Are you sure you want to quit this game without saving? (y/n)"
                ))
                GAME.state = GAMESTATE_QUIT;
            time = 0;
			break;
		case COMMAND_MISS:
			time = 120; break;
    default:
        break;
	}

	if (!isplayer && time == 0)
		time = 50; // force wait if AI can't make correct decision
	
	return time;
}

int Being::go(int dx, int dy, bool forreal) {
	int speed, item;
	int i;
	TerrainType destterrain;
	int destx = xpos + dx;
	int desty = ypos + dy;

	if (map->gettile(destx, desty).getbeing()) {
		if (isplayer) {
			if (map->gettile(destx, desty).getbeing()->type == BEING_WRECK)
				return salvage(map->gettile(destx, desty).getbeing());
			if (map->gettile(destx, desty).isbeinginlos())
				UI.msg("Bumping into them won't get the job done, you know.");
			else
				UI.msg("There seems to be something there already!");
		}
		return 0;
	}
	
	destterrain = map->gettile(destx, desty).terraintype();
	if (destterrain & habitat)
		speed = getdefaultspeed(destterrain);
	else
		speed = -1;
	item = -1;
	for (i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() == ITEM_PROPULSION && eq[i].isactive()
						&& eq[i].prop[PROPULSION_TERRAIN] & destterrain
						&& eq[i].prop[PROPULSION_SPEED] > speed) {
			speed = eq[i].prop[PROPULSION_SPEED];
			item = i;
		}
	
	if (speed < 0) {
		if (isplayer) {
			if (destterrain != TERRAIN_OFFSCREEN) {
                map->revealterrain(destx, desty);
UI.msg("You can't go there using your current propulsion system.");
			}
			else
				UI.msg("Watch out, or you'll fall off the map!");
		}
		return 0;
	}

	if (forreal) {
		map->setbeing(xpos, ypos, NULL);
		willmove();
		xpos += dx;
		ypos += dy;
		map->setbeing(destx, desty, this);
		hasmoved();

		if (item >= 0)
			damagebyuse(item);
	}

	return speedtodelay(speed);
}

int Being::salvage(Being * wreck) {
	int slot1, slot2;
	Item loot;
	char msgbuf[128];
	bool haspower;
	
	wreck->displaystatus();
	slot1 = UI.selectitem
			("Which system do you want to salvage from the wreck?", *wreck);
	if (slot1 == -1 || wreck->eq[slot1].gettype() == ITEM_VOID) {
		displaystatus();
		UI.msg("Action cancelled.");
		return 0;
	}
	loot = wreck->eq[slot1];
	strcpy(msgbuf, "Place ");
	strcat(msgbuf, loot.getname());
	strcat(msgbuf, " in which slot?");
	displaystatus();
	do {
		slot2 = UI.selectitem(msgbuf, *this);
		if (slot2 == -1) {
			UI.msg("Action cancelled.");
			return 0;
		}
		haspower = false;
		for (int i = 0; i < EQSIZE; i++)
			if (i != slot2 && eq[i].gettype() == ITEM_POWER)
				haspower = true;
		if (loot.gettype() == ITEM_POWER)
			haspower = true;
		if (!haspower)
			UI.msg("Your robot needs a source of power!");
		else
			break;
	} while (true);
	wreck->eq[slot1] = eq[slot2];
	wreck->checkexistence();
	willmove();
	eq[slot2] = loot;
	hasmoved();
	displaystatus();
	return 300;
}

int Being::attack(int slotno) {
	Being * attackee = NULL;
	int x, y;
	bool fire;
	
	if (isplayer) {
		UI.msg("Use +/- to cycle through targets; x to cancel; any other key to fire"); // FIXME if redefinable keys
		map->visitLOS(xpos, ypos, eq[slotno].prop[WEAPON_RANGE],
						eq[slotno].prop[WEAPON_TERRAIN], &Map::setvalid);
		fire = target(x, y, true);
		map->visitLOS(xpos, ypos, eq[slotno].prop[WEAPON_RANGE],
						eq[slotno].prop[WEAPON_TERRAIN], &Map::resetvalid);
		if (!fire) {
			UI.msg("Action cancelled.");
			return 0;
		}

		attackee = map->gettile(x, y).getbeing();
		if (attackee == this) {
			UI.msg("Your safety protocols prevent you from shooting yourself.");
			return 0;
		}
	}
	else
		attackee = BEING; // ie the player
	
	if (!attackee) {
		if (isplayer)
			UI.msg("You don't hit anything.");
	}
	else {
		// animation
		// UI.display(DISPLAYTOP + x, DISPLAYLEFT + y, '*', COLOR_LYELLOW);
		attackee->takehit(eq[slotno].prop[WEAPON_POWER],
						eq[slotno].prop[WEAPON_DAMAGETYPE], this);
		// napms(300);
		// map->display(x, y);
	}

	damagebyuse(slotno);
	
	return 120;
}

// returns true if being is destroyed
bool Being::takehit(int power, int damagetype, Being * attacker) {
	int cdf[EQSIZE];
	int i, j;
	int armor = 0, damage;
	bool showattacker = false;
	char msgbuf[128];

	if (isplayer) {
		if (!map->gettile(attacker->xpos, attacker->ypos).isbeinginlos())
			showattacker = true;
	}
	else {
		// AI info
		guessx = attacker->xpos;
		guessy = attacker->ypos;
		thinksplayeristhere = true;
		timesincelasthit = 0;
	}
	
	j = 0;
	// items of type matching damage type have triple chance of being hit
	for (i = 0; i < EQSIZE; i++) {
		switch (eq[i].gettype()) {
			case ITEM_VOID:
				break;
			case ITEM_POWER:
				if (damagetype == DAMAGE_POWER) j+=3; else j++; break;
			case ITEM_WEAPON:
				if (damagetype == DAMAGE_WEAPON) j+=3; else j++; break;
			case ITEM_PROPULSION:
				if (damagetype == DAMAGE_PROPULSION) j+=3; else j++; break;
			case ITEM_SENSOR:
				if (damagetype == DAMAGE_SENSOR) j+=3; else j++; break;
			case ITEM_ARMOR:
				if (damagetype == DAMAGE_ARMOR) j+=3; else j++;
				if (eq[i].isactive()) {
					int thisarmor = eq[i].prop[ARMOR_STRENGTH];
					if (eq[i].prop[ARMOR_RESISTANCE] & damagetype)
						thisarmor *= 2;
					if (thisarmor > armor)
						armor = thisarmor;
				}
				break;
			default:
				if (damagetype == DAMAGE_OTHER) j+=3; else j++; break;
		}
		cdf[i] = j;
	}
	j = rng(j);
	for (i = 0; cdf[i] <= j; i++); // should be binary search ;-)
	damage = power + rng(power / 2 + 1) - armor - rng(armor / 2 + 1);
	// Assume all combat involves player, so always print a message
	if (damage > 0) {
		bool powerhit = (eq[i].gettype() == ITEM_POWER);
		if (isplayer) {
			strcpy(msgbuf, "The ");
			strcat(msgbuf, attacker->getname());
			strcat(msgbuf, " hits your ");
			strcat(msgbuf, eq[i].getname());
		}
		else {
			strcpy(msgbuf, "You hit the ");
			strcat(msgbuf, getname());
			strcat(msgbuf, "'s ");
			strcat(msgbuf, eq[i].getname());
		}
		bool redraw = (isplayer && eq[i].gettype() == ITEM_SENSOR
						&& eq[i].isactive()
						&& eq[i].status - damage <= 50);
		if (redraw)
			willmove();
		if (eq[i].receivedamage(damage)) {
			if (eq[i].gettype() == ITEM_VOID) {
				// item hit and destroyed
				strcat(msgbuf, ", destroying it!");
				if (powerhit) {
					// power core hit and destroyed
					turntowreck();
					if (isplayer)
						strcat(msgbuf, " Your robot explodes! Game over...");
					else
						strcat(msgbuf, " The enemy explodes!");
				}
				checkexistence();
			}
			else
				// item knocked off-line
				strcat(msgbuf, ", knocking it off-line.");
		}
		else
			// received damage, but not destroyed or knocked off-line
			strcat(msgbuf, ".");
		if (isplayer)
			displayitemslot(i);
		if (redraw)
			hasmoved();
	}
	else {
		// received no damage
		if (isplayer) {
			strcpy(msgbuf, "The ");
			strcat(msgbuf, attacker->getname());
			strcat(msgbuf, " hits you but doesn't do any damage.");
		}
		else {
			strcpy(msgbuf, "You hit the ");
			strcat(msgbuf, getname());
			strcat(msgbuf, " but don't do any damage.");
		}
	}
	if (showattacker) {
		char symbol;
		int fore;
		
		UI.query(msgbuf, COLOR_WHITE); // forces display to flush
		attacker->getappearance(symbol, fore);
		UI.display(DISPLAYTOP + attacker->xpos, DISPLAYLEFT + attacker->ypos,
						symbol, fore);
		UI.moreprompt();
		map->display(attacker->xpos, attacker->ypos);
	}
	else
		UI.msg(msgbuf);
	return false;
}

int Being::energycycle() {
	int max = 0, num = 0;

	for (int i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() == ITEM_POWER && eq[i].isactive()) {
			if (eq[i].prop[POWER_ANY] > max)
				max = eq[i].prop[POWER_ANY];
			num++;
		}
	
	if (num == 0)
		return 160; // time until next check
	
	if (checkrepairing())
		repair(repairing, 1);
	
	return speedtodelay(max + num - 1);
}

int Being::selfenergycycle() {
	for (int i = 0; i < EQSIZE; i++) {
		if (eq[i].gettype() == ITEM_POWER && !eq[i].isactive()
						&& eq[i].status < 100)
			repair(i, 1);
		if ((eq[i].gettype() == ITEM_POWER || eq[i].gettype() == ITEM_SENSOR
						|| eq[i].gettype() == ITEM_ARMOR) && eq[i].isactive())
			damagebyuse(i);
	}

	return 160;
}

bool Being::checkrepairing() {
	int num = 0;

	if (isplayer && repairing >= 0 && eq[repairing].gettype() != ITEM_VOID
                 && eq[repairing].status < 100)
		return true;
	
	for (int i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() != ITEM_VOID && eq[i].status < 100) {
			num++;
			repairing = i;
		}
	if (num == 0)
		return false;

	if (num > 1)
		selectrepairing(true);
	
	return true;
}

void Being::selectrepairing(bool autoselect) {
	if (isplayer && !autoselect) {
		bool done = true;
        int newrepairing;
		
		for (int i = 0; i < EQSIZE; i++)
			if (eq[i].gettype() != ITEM_VOID && eq[i].status < 100)
				done = false;
		if (done)
			UI.msg("All systems are in perfect condition.");
		else {
			while (!done) {
                newrepairing = UI.selectitem
                    ("Which system should be repaired next?", *this);
				if (newrepairing < 0) {
                    if (repairing < 0
                        || eq[repairing].gettype() == ITEM_VOID
                        || eq[repairing].status == 100)
                        selectrepairing(true);
                    // else keep repairing unchanged
                    done = true;
                }
                else if (eq[newrepairing].gettype() != ITEM_VOID
                         && eq[newrepairing].status < 100) {
                    repairing = newrepairing;
					done = true;
                }
			}
		}
	}
	else {
		int j = 100;
		for (int i = 0; i < EQSIZE; i++)
			if (eq[i].gettype() != ITEM_VOID && eq[i].status < j) {
				j = eq[i].status;
				repairing = i;
			}
	}
}

void Being::repair(int slotno, int amount) {
	char msgbuf[128];

	// amount is always 1 for now
	eq[slotno].status++;
	
	if (isplayer) {
		switch (eq[slotno].status) {
			case 51:
					strcpy(msgbuf, "Your ");
					strcat(msgbuf, eq[slotno].getname());
					strcat(msgbuf, " is back online.");
					if (isplayer && eq[slotno].gettype() == ITEM_SENSOR)
						hasmoved();
					UI.msg(msgbuf);
				break;
			case 100:
				if (isplayer) {
					strcpy(msgbuf, "Your ");
					strcat(msgbuf, eq[slotno].getname());
					strcat(msgbuf, " is completely restored.");
					UI.msg(msgbuf);
				}
				break;
		}
		displayitemslot(slotno);
	}
}

void Being::damagebyuse(int slotno) {
	if (eq[slotno].energyuse == 0)
		return;

	char msgbuf[128];
	bool powerhit = (eq[slotno].gettype() == ITEM_POWER);
	bool redraw = (isplayer && eq[slotno].gettype() == ITEM_SENSOR
					&& eq[slotno].isactive()
					&& eq[slotno].status - eq[slotno].energyuse <= 50);
	
	if (redraw)
		willmove();
	if (eq[slotno].receivedamage(eq[slotno].energyuse)) {
		if (isplayer) {
			strcpy(msgbuf, "Your ");
			strcat(msgbuf, eq[slotno].getname());
		}
		if (eq[slotno].gettype() == ITEM_VOID) {
			if (isplayer)
				strcat(msgbuf, " overloads and is destroyed!");
			if (powerhit) {
				turntowreck();
				if (isplayer)
					strcat(msgbuf, " Your robot explodes! Game over...");
				else
					strcpy(msgbuf, "The enemy's power core overloads!");
			}
			checkexistence();
		}
		else if (isplayer)
			strcat(msgbuf, " overloads and is off-line.");
		if (isplayer || (eq[slotno].gettype() == ITEM_VOID && powerhit))
			UI.msg(msgbuf);
	}
	if (isplayer)
		displayitemslot(slotno);
	if (redraw)
		hasmoved();
}

int Being::reorderitems() {
	int slot1, slot2;
	Item tmp;
	char msgbuf[128];
	
	slot1 = UI.selectitem
			("Which system do you want move to another position?", *this);
	if (slot1 == -1 || eq[slot1].gettype() == ITEM_VOID) {
		UI.msg("Action cancelled.");
		return 0;
	}
	tmp = eq[slot1];
	strcpy(msgbuf, "Move ");
	strcat(msgbuf, tmp.getname());
	strcat(msgbuf, " to which slot?");
	slot2 = UI.selectitem(msgbuf, *this);
	if (slot2 == -1 || slot2 == slot1) {
		UI.msg("Action cancelled.");
		return 0;
	}
	eq[slot1] = eq[slot2];
	eq[slot2] = tmp;
	displayitemslot(slot1);
    displayitemslot(slot2);
    if (repairing == slot1)
        repairing = slot2;
    else if (repairing == slot2)
        repairing = slot1;
	
    return 0;
}

int Being::onoff() {
	int item;
	
	if (isplayer) {
		item = UI.selectitem("Which item do you want to switch on or off?",
                             *this);
		if (item == -1) {
			UI.msg("Action cancelled.");
			return 0;
		}
		if (eq[item].gettype() == ITEM_SENSOR)
			willmove();
		if (eq[item].gettype() == ITEM_POWER
						|| eq[item].gettype() == ITEM_PROPULSION
						|| eq[item].gettype() == ITEM_SENSOR
						|| eq[item].gettype() == ITEM_ARMOR) {
			eq[item].active = !eq[item].active;
			displayitemslot(item);
			if (eq[item].active)
				damagebyuse(item);
		}
		else if (eq[item].gettype() != ITEM_VOID)
			UI.msg("That system has no on/off switch."); 
		if (eq[item].gettype() == ITEM_SENSOR)
			hasmoved();
	}

    return 0;
}

int Being::use(int slotno) {
	int time = 0;
	int x, y;
	
	switch (eq[slotno].gettype()) {
		case ITEM_POWER:
			if (!eq[slotno].isoperational())
				UI.msg("Warning: this power core can currently only repair itself.");
			else if (!eq[slotno].isactive())
				UI.msg("Warning: this power core will only repair itself while it is switched off.");
			selectrepairing(false); // FIXME: should be option
			break;
		case ITEM_WEAPON:
			if (eq[slotno].isoperational())
				time = attack(slotno);
			else if (isplayer)
				UI.msg("That system is in no state to be used.");
			break;
		case ITEM_PROPULSION:
			if (isplayer)
				UI.msg("Press movement keys to use your propulsion system.");
			break;
		case ITEM_SENSOR:
			if (isplayer) {
				UI.msg("Looking around...");
				target(x, y, false);
			}
			break;
		case ITEM_ARMOR:
			if (isplayer)
				UI.msg("Your armor functions constantly while it is active.");
			break;
    case ITEM_VOID:
        // ignore keys corresponding to nonexisting items
        break;
    default:
        UI.msg("Use of that item is not implemented yet!");
        break;
	}
	
	return time;
}

bool Being::target(int & x, int & y, bool mustbevalid) {
	Command c;
	int i;
	bool done = false;
	bool confirm;
	
	if (prevtarget != 0
        && BEING[prevtarget].type != BEING_VOID
        && map->gettile(BEING[prevtarget].xpos,
                        BEING[prevtarget].ypos).isbeinginlos()) {
		x = BEING[prevtarget].xpos;
		y = BEING[prevtarget].ypos;
		if (mustbevalid && !map->gettile(x, y).isvalid()) {
            move(0, 0);
            clrtoeol();
            move(0, 0);
			addstr("Invalid target.");
        }
		map->gettile(x, y).getbeing()->displaystatus();
	}
	else if (mustbevalid) {
        // auto-select a target
        cycletarget(x, y, 1, mustbevalid);
        // only select player if nothing else around
        if (prevtarget == 0)
            cycletarget(x, y, 1, mustbevalid);
        map->gettile(x, y).getbeing()->displaystatus();
    }
    else {
		prevtarget = 0;
		x = xpos;
		y = ypos;
	}

	while (true) {
		move(DISPLAYTOP + x, DISPLAYLEFT + y);
		c = UI.getcommand();
		switch (c) {
			case COMMAND_NORTH:
				x--; break;
			case COMMAND_NE:
				x--; y++; break;
			case COMMAND_EAST:
				y++; break;
			case COMMAND_SE:
				x++; y++; break;
			case COMMAND_SOUTH:
				x++; break;
			case COMMAND_SW:
				x++; y--; break;
			case COMMAND_WEST:
				y--; break;
			case COMMAND_NW:
				x--; y--; break;
        case COMMAND_DETAILS:
            if (map->gettile(x, y).isbeinginlos()
                    && map->gettile(x, y).getbeing()) {
                UI.detailspage(*map->gettile(x, y).getbeing());
                GAME.normaldisplay();
            }
            break;
			case COMMAND_CANCEL:
				done = true; confirm = false; break;
			case COMMAND_NEXT:
                cycletarget(x, y, 1, mustbevalid);
				break;
        case COMMAND_PREV:
            cycletarget(x, y, -1, mustbevalid);
            break;
			default:
				if (!mustbevalid || map->gettile(x, y).isvalid()) {
					done = true; confirm = true; break;
                }
		}

        if (done)
            break;

		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= DISPLAYHEIGHT) x = DISPLAYHEIGHT - 1;
		if (y >= DISPLAYWIDTH) y = DISPLAYWIDTH - 1;
	
		if (map->gettile(x, y).getbeing()
                && map->gettile(x, y).isbeinginlos()
                && (!mustbevalid || map->gettile(x, y).isvalid()))
			for (i = 0; i < MAXNUMBEINGS; i++)
				if (BEING[i].xpos == x && BEING[i].ypos == y) {
					prevtarget = i;
					break;
				}
		
		UI.clearstatus();
		
		move(0, 0);
		clrtoeol();
		move(0, 0);
		if (mustbevalid && !map->gettile(x, y).isvalid()) {
			addstr("Invalid target.");
			if ((map->gettile(x, y).isbeinginlos())
							&& map->gettile(x, y).getbeing())
				map->gettile(x, y).getbeing()->displaystatus();
        }
		else if (!map->gettile(x, y).isseen())
			addstr("You haven't seen this location yet.");
		else {
			if (map->gettile(x, y).isterraininlos())
				addstr("You see: ");
			else
				addstr("You remember: ");
			addstr(map->gettile(x, y).getname());
			if (map->gettile(x, y).isbeinginlos()
							&& map->gettile(x, y).getbeing()) {
				addstr(", ");
				addstr(map->gettile(x, y).getbeing()->getname());
				map->gettile(x, y).getbeing()->displaystatus();
			}
		}
	}
	
	displaystatus();
	move(0, 0);
	clrtoeol();
	
	return confirm;
}

void Being::cycletarget(int & x, int & y, int dir, bool mustbevalid) {
    if (dir < 0)
        dir += MAXNUMBEINGS;

    do {
        prevtarget = (prevtarget + dir) % MAXNUMBEINGS;
        x = BEING[prevtarget].xpos;
        y = BEING[prevtarget].ypos;
    } while (!(BEING[prevtarget].type != BEING_VOID
               && map->gettile(x, y).isbeinginlos()
               && (!mustbevalid
                   || map->gettile(x, y).isvalid())));
}

// Design goal for AI: don't try to find the best possible move, but behave
// in such a way that all items can make a difference in a way visible to
// the player.
// Also, don't cheat. :-)
Command Being::AI() {
	Command c = COMMAND_UNDEFINED;
	StrategyType strategy;
	int selfeval = 0;
	int dx = 0, dy = 0, use = -1;
	int i;
	bool willtrytofire = thinksplayeristhere;
	bool wantspower = false;

	timesincelasthit++;

	// check sensors
	for (i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() == ITEM_SENSOR && eq[i].isactive()) {
			if (map->checkLOS(xpos, ypos, eq[i].prop[SENSOR_RANGE],
							eq[i].prop[SENSOR_TERRAIN],
							BEING->xpos, BEING->ypos)) {
				guessx = BEING->xpos;
				guessy = BEING->ypos;
				thinksplayeristhere = true;
				willtrytofire = true;
				break;
			}
			if (thinksplayeristhere && map->checkLOS(xpos, ypos,
							eq[i].prop[SENSOR_RANGE],
							eq[i].prop[SENSOR_TERRAIN],
							guessx, guessy))
				willtrytofire = false;
		}

	// evaluate own strength in combat
	for (i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() != ITEM_VOID)
			if (eq[i].isoperational()) {
				selfeval++;
				if (eq[i].status < 100)
					wantspower = true;
			}
			else
				selfeval--;

	// switch power on/off
	for (i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() == ITEM_POWER) {
			if (!wantspower && eq[i].energyuse > 0)
				eq[i].active = false;
			else if (eq[i].active == false) {
				// switch power on if it seems sensible
				if (eq[i].status > 50 + 3 * eq[i].energyuse) {
					eq[i].active = true;
					damagebyuse(i);
				}
			}
		}
	
	// choose strategy
	if (selfeval <= 0) {
		if (timesincelasthit <= 1)
			strategy = AI_EMERGENCY;
		else if (timesincelasthit <= 16)
			strategy = AI_RUN;
		else
			strategy = AI_REST;
	}
	else
		strategy = AI_ATTACK;

	if (strategy == AI_EMERGENCY) {
		// TODO: implement emergency items
		strategy = AI_RUN;
	}

	bool hasweapons = true;
	do {
	if (strategy == AI_RUN) {
		if (!thinksplayeristhere)
			strategy = AI_REST;
		else if (!mobile)
			strategy = AI_GUARD;
		else {
			for (i = 0; i < 24; i++) {
				dx = rng(3) - 1;
				dy = rng(3) - 1;
				if (dist(xpos + dx, ypos + dy, guessx, guessy) + (i<12?0:1)
								> dist(xpos, ypos, guessx, guessy)
								&& go(dx, dy, false) != 0) {
					c = COMMAND_NORTH;
					break;
				}
			}
			if (c == COMMAND_UNDEFINED)
				if (hasweapons)
					strategy = AI_ATTACK;
				else
					strategy = AI_GUARD;
		}
	}
	
	if (strategy == AI_REST) {
		c = COMMAND_WAIT;
		for (i = 0; i < EQSIZE; i++) {
			if (eq[i].gettype() == ITEM_ARMOR && eq[i].energyuse > 0)
				eq[i].active = false;
			else if (eq[i].gettype() == ITEM_SENSOR) {
				if (eq[i].status == 100 && eq[i].active == false) {
					eq[i].active = true;
					damagebyuse(i);
				}
				else if (eq[i].energyuse > 0)
					eq[i].active = false;
			}
		}
		
	}
	else {
		// switch items on
		for (i = 0; i < EQSIZE; i++)
			if (eq[i].gettype() == ITEM_ARMOR || eq[i].gettype() == ITEM_SENSOR
							&& eq[i].active == false
							&& eq[i].status - 3 * eq[i].energyuse > 50) {
				eq[i].active = true;
				damagebyuse(i);
			}
	}
	
	if (strategy == AI_ATTACK) {
		if (thinksplayeristhere && willtrytofire) {
			int j = 0;
			hasweapons = false;
			for (i = 0; i < EQSIZE; i++)
				if (eq[i].gettype() == ITEM_WEAPON && eq[i].isactive()) {
					hasweapons = true;
					if (map->checkLOS(xpos, ypos, eq[i].prop[WEAPON_RANGE],
											eq[i].prop[WEAPON_TERRAIN],
											guessx, guessy)) {
						j++;
						c = COMMAND_1;
						if (rng(j) == 0)
							use = i;
					}
				}
			if (c == COMMAND_UNDEFINED) {
				if (hasweapons)
					strategy = AI_APPROACH;
				else
					strategy = AI_RUN;
			}
			else {
				if (BEING->xpos != guessx || BEING->ypos != guessy) {
					c = COMMAND_MISS;
					thinksplayeristhere = false;
					if (map->gettile(guessx, guessy).isterraininlos()) {
						char msgbuf[128];
						strcpy(msgbuf, "The ");
						strcat(msgbuf, getname());
						strcat(msgbuf, " fires near your coordinates!");
						UI.msg(msgbuf);
					}
				}
			}
		}
	}
	} while (c == COMMAND_UNDEFINED && strategy == AI_RUN);
	
	if (strategy == AI_APPROACH) {
		if (!thinksplayeristhere || !mobile)
			strategy = AI_GUARD;
		else {
			for (i = 0; i < 24; i++) {
				dx = rng(3) - 1;
				dy = rng(3) - 1;
				if (dist(xpos + dx, ypos + dy, guessx, guessy) - (i<12?0:1)
								< dist(xpos, ypos, guessx, guessy)
								&& go(dx, dy, false) != 0) {
					c = COMMAND_NORTH;
					break;
				}
			}
			if (c == COMMAND_UNDEFINED)
				strategy = AI_GUARD;
		}
	}
	
	if (strategy == AI_GUARD)
		c = COMMAND_WAIT;
	
	if (c == COMMAND_NORTH)
		c = (dx < 0 ?
		(dy < 0 ? COMMAND_NW : (dy > 0 ? COMMAND_NE : COMMAND_NORTH))
		: (dx > 0 ?
		(dy < 0 ? COMMAND_SW : (dy > 0 ? COMMAND_SE : COMMAND_SOUTH))
		: (dy < 0 ? COMMAND_WEST : (dy > 0 ? COMMAND_EAST : COMMAND_WAIT))));
	if (c == COMMAND_1)
		switch (use) {
			case 0: c = COMMAND_1; break;
			case 1: c = COMMAND_2; break;
			case 2: c = COMMAND_3; break;
			case 3: c = COMMAND_4; break;
			case 4: c = COMMAND_5; break;
			case 5: c = COMMAND_6; break;
			case 6: c = COMMAND_7; break;
			case 7: c = COMMAND_8; break;
			default: c = COMMAND_WAIT; break;
		}

	return c;
}

void Being::visitLOSforeachsensor(visitor visit) {
	(map->*visit)(xpos, ypos);
	for (int i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() == ITEM_SENSOR && eq[i].isactive())
			map->visitLOS(xpos, ypos, eq[i].prop[SENSOR_RANGE],
							eq[i].prop[SENSOR_TERRAIN], visit);
}

void Being::willmove() {
	if (isplayer)
		visitLOSforeachsensor(&Map::hide);
	else
		if (map->gettile(xpos, ypos).isterraininlos())
			map->display(xpos, ypos);
}

void Being::hasmoved() {
	if (isplayer)
		visitLOSforeachsensor(&Map::reveal);
	else 
		if (map->gettile(xpos, ypos).isbeinginlos())
			map->display(xpos, ypos);
}

void Being::displaystatus() {
	for (int i = 0; i < EQSIZE; i++)
		displayitemslot(i);
}

void Being::displayitemslot(int itemno) {
	if (eq[itemno].gettype() != ITEM_VOID)
		UI.displayitemslot(itemno, eq[itemno]);
	else
		UI.clearitemslot(itemno);
}

void Being::turntowreck() {
	int i;
	
	type = BEING_WRECK;
	map->display(xpos, ypos);
	// remove events related to this being from the queue
	for (i = 0; i < QUEUE->getsize(); i++)
		while (QUEUE->peek(i).being == getid() && i < QUEUE->getsize())
			QUEUE->pop(i--);
}

void Being::checkexistence() {
	bool itemsleft = false;
	for (int i = 0; i < EQSIZE; i++)
		if (eq[i].gettype() != ITEM_VOID)
			itemsleft = true;
	if (!itemsleft) {
		type = BEING_VOID;
		map->setbeing(xpos, ypos, NULL);
		map->display(xpos, ypos);
	}
}

int speedtodelay(int speed) {
	// calculate 160*.5^(speed/8)
	const int mult[8] = {160, 147, 135, 123, 113, 104, 95, 87};
	int delay = mult[speed & 7] >> (speed >> 3);
	if (delay < 10)
		delay = 10;
	return delay;
}

void Being::save(std::ostream & out) {
    out << type;
    if (type != BEING_VOID) {
        out << ' '
            << xpos << ' '
            << ypos << ' '
            << isplayer << ' '
            << repairing << ' '
            << std::hex << habitat << std::dec << ' '
            << mobile << ' '
            << thinksplayeristhere << ' ';
        if (thinksplayeristhere)
            out << guessx << ' '
                << guessy << ' ';
        out << timesincelasthit << '\n';
        for (int i = 0; i < EQSIZE; i++)
            eq[i].save(out);
    }
    else
        out << '\n';
}

void Being::load(std::istream & in) {
    int t;

    in >> t;
    type = (BeingType)t;
    if (type != BEING_VOID) {
        in >> xpos
           >> ypos
           >> isplayer
           >> repairing
           >> std::hex >> habitat >> std::dec
           >> mobile
           >> thinksplayeristhere;
        if (thinksplayeristhere)
            in >> guessx
               >> guessy;
        in >> timesincelasthit;
        for (int i = 0; i < EQSIZE; i++)
            eq[i].load(in);
    }
}

BeingType selectbeing(int level) {
	level += (rng(3) - rng(3));
	if (level < 0)
		level = 0;
	
	switch (level) {
		case 0: return BEING_BUGGY;
		case 1: return BEING_PROBE;
		case 2:	return BEING_SENTRY;
		case 3: return BEING_HUNTER;
		case 4: return BEING_TANK;
		case 5: return BEING_AMBUSHER;
		case 6: return BEING_SECURITY;
		case 7: return BEING_CYBORG;
		case 8: return BEING_JUGGERNAUT;
		case 9: return BEING_ARMADILLO;
		case 10: return BEING_PLANT;
		case 11: return BEING_AMPHIBIAN;
		case 12: return BEING_DESTROYER;
		case 13: return BEING_MINE;
		case 14: return BEING_BATTLESHIP;
		case 15: return BEING_GEOLOGIST;
		case 16: return BEING_BOMBER;
		case 17: return BEING_SUBMARINE;
		case 18: return BEING_DRACO;
		case 19: return BEING_LAUNCHER;
		case 20: return BEING_STEELELEMENTAL;
		default: return BEING_MECHALICH;
	}
}
