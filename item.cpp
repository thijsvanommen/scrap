#include "item.h"

Item::Item():
type(ITEM_VOID), active(ACTIVE_ON), energyuse(-1), status(100) {
    for (int i = 0; i < PROPNUM; i++)
        prop[i] = 0;
}

ItemType Item::gettype() {
    return type;
}

const char * Item::getname() {
    return name.c_str();
}

bool Item::continuousenergyuser() {
    return (energyuse >= 0 &&
            (gettype() == ITEM_POWER
             || gettype() == ITEM_SENSOR
             || gettype() == ITEM_ARMOR));
}

bool Item::isoperational() {
    return (status > 50);
}

bool Item::isactive() {
    return type != ITEM_VOID && active != ACTIVE_OFF && isoperational();
}

StatusChange Item::receivedamage(int damage, bool forreal) {
    bool wasoperational = isoperational();
    int newstatus = status - damage;

    if (forreal)
        status = newstatus;
    if (newstatus <= 0) {
        if (forreal)
            type = ITEM_VOID;
        return STATUSCHANGE_DESTROYED;
    }
    if ((newstatus > 50) != wasoperational)
        return STATUSCHANGE_OFFLINE;

    return STATUSCHANGE_NOCHANGE;
}

void Item::save(std::ostream & out) {
    out << type;
    if (type != ITEM_VOID) {
        out << ' '
            << active << ' '
            << energyuse << ' '
            << status << ' ';
        for (int i = 0; i < PROPNUM; i++)
            out << prop[i] << ' ';
        out << name;
    }
    out << '\n';
}

void Item::load(std::istream & in) {
    int t;

    in >> t;
    type = (ItemType)t;
    if (type != ITEM_VOID) {
        in >> t
           >> energyuse
           >> status;
        active = (ActiveType)t;
        for (int i = 0; i < PROPNUM; i++)
            in >> prop[i];
        in.ignore();
        getline(in, name);
    }
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
