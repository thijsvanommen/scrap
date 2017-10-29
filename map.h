#ifndef MAP_H
#define MAP_H

class Tile;
class Map;

typedef void (Map::*visitor)(int, int);

class Being;

#include <fstream>

const int MAPHEIGHT = 24;
const int MAPWIDTH = 48;

const int TILEFLAG_SEEN = 1 << 0;
const int TILEFLAG_INLOS = 1 << 1;
const int TILEFLAG_VALID = 1 << 2;

enum TileType {
    TILE_UNDEFINED,
    TILE_OFFSCREEN,
    TILE_EXIT,
    TILE_GRASS, TILE_TREE,
    TILE_DIRT, TILE_ROUGH, TILE_ROCK,
    TILE_ROAD, TILE_WALL, TILE_FLOOR,
    TILE_METALWALL, TILE_METALFLOOR,
    TILE_CAVEWALL, TILE_CAVEFLOOR,
    TILE_CHASM, TILE_SHALLOW, TILE_DEEP,
    TILE_DARK, TILE_RUIN, TILE_SUB
};

enum TerrainType {
    TERRAIN_OFFSCREEN = 0,
    TERRAIN_FLOOR    = 1 << 0,
    TERRAIN_TUNNEL    = 1 << 1,
    TERRAIN_DARK    = 1 << 2,
    TERRAIN_ROUGH    = 1 << 3,
    TERRAIN_CHASM    = 1 << 4,
    TERRAIN_SHALLOW    = 1 << 5,
    TERRAIN_DEEP    = 1 << 6,
    TERRAIN_SUB        = 1 << 7,
    TERRAIN_HIGH    = 1 << 8,
    TERRAIN_WALL    = 1 << 9,
    TERRAIN_METAL    = 1 << 10,
    TERRAIN_WILDCARD = (1 << 11) - 1
};

const int TW_NORMAL    = TERRAIN_FLOOR | TERRAIN_TUNNEL | TERRAIN_CHASM
                | TERRAIN_SHALLOW | TERRAIN_DEEP | TERRAIN_ROUGH
                | TERRAIN_DARK;
const int TW_AIR    = TW_NORMAL | TERRAIN_HIGH;
const int TW_WATER    = TERRAIN_SHALLOW | TERRAIN_DEEP | TERRAIN_SUB;
const int TW_TREMOR    = TERRAIN_WALL | TERRAIN_METAL | TERRAIN_HIGH;

const int TP_LAND    = TERRAIN_FLOOR | TERRAIN_TUNNEL | TERRAIN_DARK;
const int TP_TRACK    = TP_LAND | TERRAIN_ROUGH;
const int TP_WATER    = TERRAIN_SHALLOW | TERRAIN_DEEP;
const int TP_AIR    = TERRAIN_FLOOR | TERRAIN_CHASM | TERRAIN_SHALLOW
                | TERRAIN_DEEP | TERRAIN_ROUGH | TERRAIN_HIGH;
const int TP_SUB    = TP_WATER | TERRAIN_SUB;
const int TP_ROCK    = TERRAIN_WALL | TERRAIN_HIGH;
const int TP_DEF    = TP_LAND | TERRAIN_ROUGH | TERRAIN_SHALLOW;

const int TS_NORMAL    = TERRAIN_FLOOR | TERRAIN_TUNNEL | TERRAIN_CHASM
                | TERRAIN_SHALLOW | TERRAIN_DEEP | TERRAIN_ROUGH;
const int TS_AIR    = TS_NORMAL | TERRAIN_HIGH;
const int TS_INFRA    = TS_NORMAL | TERRAIN_DARK;
const int TS_XRAY    = TS_NORMAL | TERRAIN_WALL | TERRAIN_HIGH;
const int TS_SONAR    = TERRAIN_SHALLOW | TERRAIN_DEEP | TERRAIN_SUB;
const int TS_TREMOR    = TERRAIN_WALL | TERRAIN_METAL | TERRAIN_HIGH;

class Tile {
public:
    Tile();
    Tile(TileType newtype);
    
    void getappearance(char & symbol, int & fore, int & back);
    char * getname();
    TerrainType terraintype();
    bool oftiletype(TileType t);
    bool canpass(int flags);
    bool isseen();
    bool isterraininlos();
    bool isbeinginlos();
    bool isvalid();
    bool hasbeing();
    Being & getbeing();
    
    friend class Map;

private:
    TileType type;
    char seen;
    int being;
};

int getdefaultspeed(TerrainType terrain);

// for LOS calculations:
class Interval {
public:
    Interval();
    Interval(int newp1, int newq1, int newp2, int newq2);
    int p1, q1; // from p1/q1
    int p2, q2; // to p2/q2
};

class IntervalQueue {
public:
    IntervalQueue(int newsize);
    ~IntervalQueue();

    int getlength();
    void push(Interval newinterval);
    Interval pop();

private:
    Interval * elt;
    int front;
    int length;
    int size;
};

class Map {
public:
    bool find(TileType t, int & x, int & y);
    void settile(int x, int y, TileType t);
    void background(TileType interior, TileType border);
    void rect(int t, int b, int l, int r, TileType interior, TileType border);
    void spots(TileType spot, int num);
    void amoeba(TileType area, int size);
    void path(TileType path, int & x, int & y, int num);
    void building(TileType floor, TileType wall, int minperimeter, int sym);
    void walls(TileType wall, int t, int b, int l, int r, int minperimeter,
                    int sym, bool justsymmed = false);
    void rooms(TileType old, TileType room, int num);
    void vault(int & x, int & y);
    void chasm();
    
    void display(int x, int y);
    void displayall();
    bool inbounds(int x, int y);
    Tile gettile(int x, int y);
    void setbeing(int x, int y, Being * being);
    void visitLOS(int centerx, int centery, int radius, int terrain,
                    visitor visit);
    void visitoctant(int centerx, int centery, int radius, int terrain,
                    visitor visit, int dx, int dy, int orientation);
    bool checkLOS(int centerx, int centery, int radius, int terrain,
                    int targetx, int targety);
    void reveal(int x, int y);
    void hide(int x, int y);
    void revealterrain(int x, int y);
    void setvalid(int x, int y);
    void resetvalid(int x, int y);

    void save(std::ostream & out);
    void load(std::istream & in);
    
private:
    Tile tile[MAPHEIGHT][MAPWIDTH];
};

int dist(int x1, int y1, int x2, int y2);
char terrainsymbol(TerrainType terrain);

#endif
