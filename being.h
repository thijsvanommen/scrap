#ifndef BEING_H
#define BEING_H

class Being;

#include <fstream>
#include <vector>

#include "ui.h"
#include "item.h"
#include "map.h"

const int EQSIZE = 8;

enum BeingType {
    BEING_VOID, BEING_WRECK, BEING_PLAYER,
    BEING_BUGGY, BEING_PROBE, BEING_SENTRY, BEING_HUNTER, BEING_TANK,
    BEING_AMBUSHER, BEING_SECURITY, BEING_CYBORG, BEING_JUGGERNAUT,
    BEING_ARMADILLO, BEING_PLANT, BEING_AMPHIBIAN, BEING_DESTROYER,
    BEING_MINE, BEING_BATTLESHIP, BEING_GEOLOGIST, BEING_BOMBER,
    BEING_SUBMARINE, BEING_DRACO, BEING_LAUNCHER,
    BEING_STEELELEMENTAL, BEING_MECHALICH
};

// damage types
const int DAMAGE_NORMAL    = 1 << 0;
const int DAMAGE_POWER    = 1 << 1;
const int DAMAGE_WEAPON = 1 << 2;
const int DAMAGE_PROPULSION    = 1 << 3;
const int DAMAGE_SENSOR    = 1 << 4;
const int DAMAGE_ARMOR    = 1 << 5;
const int DAMAGE_OTHER    = 1 << 6;

enum StrategyType {
    AI_EMERGENCY, AI_RUN, AI_REST, AI_ATTACK, AI_APPROACH, AI_GUARD
};

class Being {
public:
    Being();
    Being(BeingType newType, int newid);

    void createpc();
    void place(Map * newmap, int newx, int newy);
    bool placerandom(Map * newmap);
    void getappearance(char & symbol, int & fore);
    char * getname();
    int getid();
    
    int act();
    int go(int dx, int dy, bool forreal = true);
    int salvage(Being & wreck);
    int attack(int slotno);
    void takehit(int power, int damagetype, Being * attacker);
    int energycycle();
    int itemenergycycle(int item);
    bool checkrepairing();
    void selectrepairing(bool autoselect);
    void repair(int slotno, int amount);
    void damagebyuse(int slotno, int amount);
    int reorderitems();
    void swapitems(int slot1, Being & being2, int slot2);
    int onoff();
    void switchon(int itemno);
    void switchoff(int itemno);
    void updateitemenergyevent(int itemno);
    int use(int slotno);
    bool target(int & x, int & y, bool mustbevalid);
    void cycletarget(int & x, int & y, int dir, bool mustbevalid);
    
    Command AI();

    int getrepairspeed();
    int getmovementspeed(TerrainType terrain, int & propulsionitem);
    void visitLOSforeachsensor(visitor visit);
    int getarmor(int damagetype);

    void willmove();
    void hasmoved();
    void displaystatus();
    void displayitemslot(int itemno);
    void turntowreck();
    void checkexistence();

    void save(std::ostream & out);
    void load(std::istream & in, int newid, Map * newmap);

    int xpos, ypos;
    BeingType type;
    bool isplayer;
    Item eq[EQSIZE]; // equiped items
    int repairing;
    int habitat;
    bool mobile;
    
private:
    Map * map;
    int id;

    // for AI:
    int guessx, guessy; // suspected/known player coordinates
    bool thinksplayeristhere;
    int timesinceguess;
};

int speedtodelay(int speed);
BeingType selectbeing(int level);

#endif
