#include "item.h"

Item::Item():
type(ITEM_VOID), active(true), energyuse(0), status(100) {}

ItemType Item::gettype() {
	return type;
}

char * Item::getname() {
	return name;
}

bool Item::isoperational() {
	return (status > 50);
}

bool Item::isactive() {
	return type != ITEM_VOID && active && isoperational();
}

// returns true if status change is worth a message
bool Item::receivedamage(int damage) {
	bool wasoperational = isoperational();
	status -= damage;
	if (status <= 0) {
		type = ITEM_VOID;
		return true;
	}
	return (isoperational() != wasoperational);
}

Power::Power(char * newname, int any, int energy) {
	type = ITEM_POWER;
	prop[POWER_ANY] = any;
	energyuse = energy;
	name = newname;
}

Weapon::Weapon(char * newname, int power, int damagetype, int range,
				int terrain, int energy) {
	type = ITEM_WEAPON;
	prop[WEAPON_POWER] = power;
	prop[WEAPON_DAMAGETYPE] = damagetype;
	prop[WEAPON_RANGE] = range;
	prop[WEAPON_TERRAIN] = terrain;
	energyuse = energy;
	name = newname;
}

Propulsion::Propulsion(char * newname, int speed, int terrain, int energy) {
	type = ITEM_PROPULSION;
	prop[PROPULSION_SPEED] = speed;
	prop[PROPULSION_TERRAIN] = terrain;
	energyuse = energy;
	name = newname;
}

Sensor::Sensor(char * newname, int range, int terrain, int energy) {
	type = ITEM_SENSOR;
	prop[SENSOR_RANGE] = range;
	prop[SENSOR_TERRAIN] = terrain;
	energyuse = energy;
	name = newname;
}

Armor::Armor(char * newname, int strength, int resistance, int energy) {
	type = ITEM_ARMOR;
	prop[ARMOR_STRENGTH] = strength;
	prop[ARMOR_RESISTANCE] = resistance;
	energyuse = energy;
	name = newname;
}
