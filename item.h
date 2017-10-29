#ifndef ITEM_H
#define ITEM_H

const int PROPNUM = 4;

#include <string>
#include <fstream>

enum ItemType {
	ITEM_VOID, ITEM_POWER, ITEM_WEAPON, ITEM_PROPULSION, ITEM_SENSOR,
	ITEM_ARMOR, ITEM_MISC
};

class Item {
public:
	Item();

	ItemType gettype();
	const char * getname();
	bool isoperational();
	bool isactive();
	bool receivedamage(int damage);

    void save(std::ostream & out);
    void load(std::istream & in);

	ItemType type;
	bool active;
	int energyuse;
	int status;
	int prop[PROPNUM];
	std::string name;
};

// subclasses serve mostly to make being definitions look cleaner
const int POWER_ANY = 0;

class Power : public Item {
public:
	Power(char * newname, int any, int energy = 0);
};

const int WEAPON_POWER = 0;
const int WEAPON_DAMAGETYPE = 1;
const int WEAPON_RANGE = 2;
const int WEAPON_TERRAIN = 3;

class Weapon : public Item {
public:
	Weapon(char * newname, int power, int damagetype, int range, int terrain, int energy = 0);
};

const int PROPULSION_SPEED = 0;
const int PROPULSION_TERRAIN = 1;

class Propulsion : public Item {
public:
	Propulsion(char * newname, int speed, int terrain, int energy = 0);
};

const int SENSOR_RANGE = 0;
const int SENSOR_TERRAIN = 1;

class Sensor : public Item {
public:
	Sensor(char * newname, int range, int terrain, int energy = 0);
};

const int ARMOR_STRENGTH = 0;
const int ARMOR_RESISTANCE = 1;

class Armor : public Item {
public:
	Armor(char * newname, int strength, int resistance, int energy = 0);
};
#endif
