#include "map.h"
#include "game.h"
#include "being.h"
#include "rng.h"
#include "ui.h"
extern Interface UI;

Tile::Tile():
type(TILE_UNDEFINED), seen(0), being(-1) {}

Tile::Tile(TileType newtype):
type(newtype), seen(0), being(-1) {}

void Tile::getappearance(char & symbol, int & fore, int & back) {
    symbol = terrainsymbol(terraintype());
    back = COLOR_BLACK;
    switch (type) {
        case TILE_EXIT:
            symbol = '>'; fore = COLOR_LWHITE; break;
        case TILE_GRASS:
            fore = COLOR_LGREEN; break;
        case TILE_TREE:
            fore = COLOR_GREEN; break;
        case TILE_DIRT:
            fore = COLOR_YELLOW; break;
        case TILE_ROUGH:
            fore = COLOR_RED; break;
        case TILE_ROCK:
            fore = COLOR_WHITE; break;
        case TILE_ROAD:
            fore = COLOR_RED; break;
        case TILE_WALL:
            fore = COLOR_CYAN; break;
        case TILE_FLOOR:
            fore = COLOR_CYAN; break;
        case TILE_METALWALL:
            fore = COLOR_LCYAN; break;
        case TILE_METALFLOOR:
            fore = COLOR_LCYAN; break;
        case TILE_CAVEWALL:
            fore = COLOR_MAGENTA; break;
        case TILE_CAVEFLOOR:
            fore = COLOR_MAGENTA; break;
        case TILE_CHASM:
            fore = COLOR_LBLACK; break;
        case TILE_SHALLOW:
            fore = COLOR_LBLUE; back = COLOR_BLUE; break;
        case TILE_DEEP:
            fore = COLOR_LBLUE; back = COLOR_BLUE; break;
        case TILE_DARK:
            fore = COLOR_LBLACK; break;
        case TILE_RUIN:
            fore = COLOR_WHITE; break;
        case TILE_SUB:
            fore = COLOR_LBLUE; back = COLOR_BLUE; break;
        default:
            symbol = 'E'; fore = COLOR_LYELLOW; back = COLOR_RED; break;
    }
}

char * Tile::getname() {
    switch (type) {
        case TILE_EXIT:
            return "the exit to the next level";
        case TILE_GRASS:
            return "grass";
        case TILE_TREE:
            return "tree";
        case TILE_DIRT:
            return "dirt";
        case TILE_ROUGH:
            return "rough ground";
        case TILE_ROCK:
            return "rock";
        case TILE_ROAD:
            return "road";
        case TILE_WALL:
            return "concrete wall";
        case TILE_FLOOR:
            return "concrete floor";
        case TILE_METALWALL:
            return "metal wall";
        case TILE_METALFLOOR:
            return "metal floor";
        case TILE_CAVEWALL:
            return "cavern wall";
        case TILE_CAVEFLOOR:
            return "cavern floor";
        case TILE_CHASM:
            return "deep chasm";
        case TILE_SHALLOW:
            return "shallow water";
        case TILE_DEEP:
            return "deep water";
        case TILE_DARK:
            return "darkness";
        case TILE_RUIN:
            return "ruined wall";
        case TILE_SUB:
            return "underwater";
        default:
            return "buggy tile";
    }
}

TerrainType Tile::terraintype() {
    TerrainType ret;

    switch (type) {
        case TILE_OFFSCREEN:
            ret = TERRAIN_OFFSCREEN; break;
        case TILE_FLOOR:
        case TILE_METALFLOOR:
        case TILE_CAVEFLOOR:
            ret = TERRAIN_TUNNEL; break;
        case TILE_CHASM:
            ret = TERRAIN_CHASM; break;
        case TILE_WALL:
        case TILE_CAVEWALL:
            ret = TERRAIN_WALL; break;
        case TILE_DARK:
            ret = TERRAIN_DARK; break;
        case TILE_SHALLOW:
            ret = TERRAIN_SHALLOW; break;
        case TILE_DEEP:
            ret = TERRAIN_DEEP; break;
        case TILE_ROUGH:
            ret = TERRAIN_ROUGH; break;
        case TILE_SUB:
            ret = TERRAIN_SUB; break;
        case TILE_METALWALL:
            ret = TERRAIN_METAL; break;
        case TILE_TREE:
        case TILE_ROCK:
        case TILE_RUIN:
            ret = TERRAIN_HIGH; break;
        case TILE_EXIT:
            ret = TERRAIN_WILDCARD; break;
        default:
            ret = TERRAIN_FLOOR; break;
    }

    return ret;
}

bool Tile::oftiletype(TileType t) {
    return (type == t);
}

bool Tile::canpass(int flags) {
    return (terraintype() & flags);
}

bool Tile::isseen() {
    return (seen & TILEFLAG_SEEN);
}

bool Tile::isterraininlos() {
    return (seen & TILEFLAG_INLOS);
}

// for support of cloaked beings
bool Tile::isbeinginlos() {
    return (seen & TILEFLAG_INLOS);
}

bool Tile::isvalid() {
    return (seen & TILEFLAG_VALID);
}

bool Tile::hasbeing() {
    return (being >= 0);
}

Being & Tile::getbeing() {
    return GAME.being[being];
}

int getdefaultspeed(TerrainType terrain) {
    int speed;
    
    switch(terrain) {
        case TERRAIN_FLOOR:
        case TERRAIN_TUNNEL:
        case TERRAIN_DARK:
        case TERRAIN_WILDCARD:
            speed = 2; break;
        case TERRAIN_SHALLOW:
        case TERRAIN_ROUGH:
            speed = 0; break;
        default:
            speed = -1; break;
    }
    
    return speed;
}

Interval::Interval() {};

Interval::Interval(int newp1, int newq1, int newp2, int newq2):
p1(newp1), q1(newq1), p2(newp2), q2(newq2) {}
// Don't assume that p1/q1 <= p2/q2!

IntervalQueue::IntervalQueue(int newsize):
front(0), length(0), size(newsize) {
    elt = new Interval[size];
}

IntervalQueue::~IntervalQueue() {
    delete[] elt;
}

int IntervalQueue::getlength() {
    return length;
}

void IntervalQueue::push(Interval newinterval) {
    elt[(front + length++) % size] = newinterval;
}

Interval IntervalQueue::pop() {
    Interval ret = *(elt + front);
    length--;
    front = (front + 1) % size;
    return ret;
}

bool Map::find(TileType t, int & x, int & y) {
    int tries = 0;
    
    do {
        x = rng(MAPHEIGHT);
        y = rng(MAPWIDTH);
        if (tries++ > 1000)
            return false;
    } while (tile[x][y].type != t);
    return true;
}

void Map::settile(int x, int y, TileType t) {
    tile[x][y] = Tile(t);
}

void Map::background(TileType interior, TileType border) {
    rect(0, MAPHEIGHT-1, 0, MAPWIDTH-1, interior, border);
}
    
void Map::rect(int t, int b, int l, int r,
               TileType interior, TileType border) {
    int i, j;

    for (i = t; i <= b; i++)
        tile[i][l] = tile[i][r] = Tile(border);
    for (j = l + 1; j < r; j++)
        tile[t][j] = tile[b][j] = Tile(border);
    for (i = t + 1; i < b; i++)
        for (j = l + 1; j < r; j++)
            tile[i][j] = Tile(interior);
}

void Map::spots(TileType spot, int num) {
    int x, y, u, v;
    bool free;
    
    for (int i = 0; i < num;) {
        x = rng(MAPHEIGHT);
        y = rng(MAPWIDTH);
        free = true;
        for (u = x - 1; u <= x + 1; u++)
            for (v = y - 1; v <= y + 1; v++)
                if (inbounds(u, v) && tile[u][v].type == spot)
                    free = false;
        if (free) {
            tile[x][y] = Tile(spot);
            i++;
        }
    }
}

void Map::amoeba(TileType area, int size) {
    int x, y, u, v;
    bool connected;

    for (int i = 0; i < size;) {
        x = rng(MAPHEIGHT);
        y = rng(MAPWIDTH);
        if (tile[x][y].type == area)
            continue;
        connected = false;
        for (u = x - 1; u <= x + 1; u++)
            for (v = y - 1; v <= y + 1; v++)
                if (inbounds(u, v) && tile[u][v].type == area)
                    connected = true;
        if (connected) {
            tile[x][y] = Tile(area);
            i++;
        }
    }
}

void Map::path(TileType path, int & x, int & y, int num) {
    int i, j, dist, dir;
    
    if (x == -1) {
        x = rng(MAPHEIGHT);
        y = rng(MAPWIDTH);
    }
    tile[x][y] = Tile(path);
    for (i = 0; i < num; i++) {
        dist = rng(12) + 3;
        dir = (rng(2) == 0 ? 1 : -1);
        if (!inbounds(x, y + dist*dir))
            dir = -dir;
        for (j = 0; j < dist; j++) {
            y += dir;
            tile[x][y] = Tile(path);
        }
        dist = rng(8) + 3;
        dir = (rng(2) == 0 ? 1 : -1);
        if (!inbounds(x + dist*dir, y))
            dir = -dir;
        for (j = 0; j < dist; j++) {
            x += dir;
            tile[x][y] = Tile(path);
        }
    }
}

void Map::building(TileType floor, TileType wall, int minperimeter, int sym) {
    background(floor, wall);
    walls(wall, 0, MAPHEIGHT - 1, 0, MAPWIDTH - 1, minperimeter, sym);
}

void Map::walls(TileType wall, int t, int b, int l, int r, int minperimeter,
                int sym, bool justsymmed) {
    int x, y, i;
    bool horizontal = false;
    
    if (b - t > r - l)
        horizontal = true;
    else if (b - t == r - l && rng(2) == 0)
        horizontal = true;

    if (!justsymmed && b - t + r - l < minperimeter && rng(2) == 1)
        return;
    
    if (horizontal) {
        if (b - t < 6)
            return;
        if (!justsymmed && rng(100) < sym) {
            walls(wall, t, (t+b+3)/2, l, r, minperimeter, sym, true);
            // produce mirror image
            for (x = (t+b+3)/2; x < b; x++)
                for (y = l + 1; y < r; y++)
                    tile[x][y] = Tile(tile[t+b-x][y].type);
        }
        else {
            x = rng(b - t - 5) + t + 3;
            y = rng(r - l - 1) + l + 1;
            for (i = l + 1; i < r; i++)
                if (i != y)
                    tile[x][i] = Tile(wall);
            walls(wall, t, x, l, r, minperimeter, sym);
            walls(wall, x, b, l, r, minperimeter, sym);
        }
    }
    else {
        if (r - l < 6)
            return;
        if (!justsymmed && rng(100) < sym) {
            walls(wall, t, b, l, (l+r+3)/2, minperimeter, sym, true);
            // produce mirror image
            for (x = t + 1; x < b; x++)
                for (y = (l+r+3)/2; y < r; y++)
                    tile[x][y] = Tile(tile[x][l+r-y].type);
        }
        else {
            x = rng(b - t - 1) + t + 1;
            y = rng(r - l - 5) + l + 3;
            for (i = t + 1; i < b; i++)
                if (i != x)
                    tile[i][y] = Tile(wall);
            walls(wall, t, b, l, y, minperimeter, sym);
            walls(wall, t, b, y, r, minperimeter, sym);
        }
    }
}

void Map::rooms(TileType old, TileType room, int num) {
    int x, y, i, t, b, l, r;
    
    for (i = 0; i < num; i++) {
        if (!find(old, x, y))
            return;
        t = x - rng(3) - 1;
        b = x + rng(3) + 1;
        l = y - rng(4) - 1;
        r = y + rng(4) + 1;
        if (t < 0) t = 0;
        if (b >= MAPHEIGHT) b = MAPHEIGHT - 1;
        if (l < 0) l = 0;
        if (r >= MAPWIDTH) r = MAPWIDTH - 1;
        for (x = t; x <= b; x++)
            for (y = l; y <= r; y++)
                tile[x][y] = Tile(room);
    }
}

void Map::vault(int & x, int & y) {
    x = rng(MAPHEIGHT - 10) + 3;
    y = rng(MAPWIDTH - 11) + 3;
    rect(x, x + 4, y, y + 5, TILE_METALFLOOR, TILE_METALWALL);
    if (rng(2) == 0)
        tile[x + 2][y] = Tile(TILE_METALFLOOR);
    else
        tile[x + 2][y + 5] = Tile(TILE_METALFLOOR);
}

void Map::chasm() {
    int x, y, l = 1 + rng(1);
    
    for (x = 0; x < MAPHEIGHT; x++) {
        switch (rng(6)) {
            case 0:
                if (l > 0) l--; break;
            case 1:
                if (l < 3) l++; break;
        }
        for (y = 0; y < 3; y++)
            tile[x][MAPWIDTH / 2 - 2 + l + y] = Tile(TILE_CHASM);
    }
}

void Map::display(int x, int y) {
    char symbol;
    int fore, back;

    if (tile[x][y].isseen()) {
        tile[x][y].getappearance(symbol, fore, back);
        if (!tile[x][y].isterraininlos()) {
             fore = COLOR_WHITE;
             back = COLOR_BLACK;
        }
        if (tile[x][y].hasbeing() && tile[x][y].isbeinginlos())
            tile[x][y].getbeing().getappearance(symbol, fore);
    }
    else {
        fore = COLOR_WHITE;
        back = COLOR_BLACK;
        symbol = ' ';
    }
    UI.display(DISPLAYTOP + x, DISPLAYLEFT + y, symbol, fore, back);
}

void Map::displayall() {
    int i, j;
    
    for (i = 0; i < MAPHEIGHT; i++)
        for (j = 0; j < MAPWIDTH; j++)
            display(i, j);
}

bool Map::inbounds(int x, int y) {
    return (x >= 0 && y >= 0 && x < MAPHEIGHT && y < MAPWIDTH);
}

Tile Map::gettile(int x, int y) {
    if (inbounds(x, y))
        return tile[x][y];
    else
        return Tile(TILE_OFFSCREEN);
}

void Map::setbeing(int x, int y, Being * newbeing) {
        tile[x][y].being = (newbeing ? newbeing->getid() : -1);
}

// visit all squares with a clear line of sight from the center of the
// viewer's square at least once (possibly twice for orthogonals and
// diagonals)
void Map::visitLOS(int centerx, int centery, int radius, int terrain,
                visitor visit) {
    (this->*visit)(centerx, centery);
    if (radius > 0 && tile[centerx][centery].canpass(terrain)) {
        int i, j;
        const int dx[4] = {0, -1,  0, 1};
        const int dy[4] = {1,  0, -1, 0};
        
        for (i = 0; i < 4; i++)
            for (j = -1; j <= 1; j += 2)
                visitoctant(centerx, centery, radius, terrain, visit,
                                dx[i], dy[i], j);
    }
}

void Map::visitoctant(int centerx, int centery, int radius, int terrain,
                visitor visit, int dx, int dy, int orientation) {
    int x, y, u, v, vmax;
    int i;
    IntervalQueue q(radius); // could do with 1/2 size
    Interval cur, tmp;
    bool addtmp;

    q.push(Interval(0, 1, 255, 256)); // 0 and 1-epsilon

    for (u = 1; u <= radius && q.getlength() > 0; u++) {
        for (i = q.getlength(); i > 0; i--) {
            cur = q.pop();
            v = (int)((cur.p1-cur.q1)*(2*u-1)/(2*cur.q1))+u-1;
            vmax = (int)((cur.p2*(2*u+1)+cur.q2)/(2*cur.q2));
            x = centerx + dx*u - orientation*dy*v;
            y = centery + dy*u + orientation*dx*v;
            addtmp = false;
            for (; v <= vmax; v++) {
                if (!inbounds(x, y))
                    break;
                (this->*visit)(x, y);
                if (tile[x][y].canpass(terrain)) {
                    if (!addtmp) {
                        // set start of tmp interval
                        tmp.p1 = 2*v-1;
                        tmp.q1 = 2*u-1;
                        if (cur.p1*tmp.q1 > tmp.p1*cur.q1) {
                            tmp.p1 = cur.p1;
                            tmp.q1 = cur.q1;
                        }
                        addtmp = true;
                    }
                }
                else {
                    if (addtmp) {
                        // set end of tmp interval
                        tmp.p2 = 2*v-1;
                        tmp.q2 = 2*u+1;
                        // known to be < cur.{p/q}2
                        q.push(tmp);
                        addtmp = false;
                    }
                }
                x -= orientation*dy;
                y += orientation*dx;
            } // for v
            if (addtmp) {
                tmp.p2 = cur.p2;
                tmp.q2 = cur.q2;
                q.push(tmp);
            }
        } // for i
    } // for u
}

// checks for two given points if a line of sight exists from one to the
// other (as specified above)
bool Map::checkLOS(int centerx, int centery, int radius, int terrain,
                int targetx, int targety) {
    if (centerx == targetx && centery == targety)
        return true;
    if (centerx - targetx > radius || targetx - centerx > radius
                    || centery - targety > radius
                    || targety - centery > radius)
        return false;
    if (!tile[centerx][centery].canpass(terrain))
        return false;
    
    int dx, dy, absdx, absdy, orientation;
    int maxd, mind;
    Interval cur;
    
    absdx = targetx - centerx;
    absdy = targety - centery;
    // determine dx, dy and orientation of the relevant octant
    if (absdx < 0) {
        absdx = -absdx;
        dx = -1;
    }
    else
        dx = 1;
    if (absdy < 0) {
        absdy = -absdy;
        dy = -1;
    }
    else
        dy = 1;
    if (absdx == absdy) {
        // diagonal
        cur.p1 = cur.q1 = cur.p2 = cur.q2 = 1;
        dx = (dx + dy) / 2;
        dy -= dx;
        orientation = 1;
        maxd = mind = absdx;
    }
    else {
        if (absdx > absdy) {
            orientation = dx*dy;
            dy = 0;
            maxd = absdx;
            mind = absdy;
        }
        else {
            orientation = -dx*dy;
            dx = 0;
            maxd = absdy;
            mind = absdx;
        }
        if (mind == 0) {
            cur.p1 = 0; cur.q1 = 1; cur.p2 = 0; cur.q2 = 1;
        }
        else {
            cur.p1 = 2*mind-1;
            cur.q1 = 2*maxd+1;
            cur.p2 = 2*mind+1;
            cur.q2 = 2*maxd-1;
        }
    }
    
    int x, y, u, v;
    bool seenspace, seenblock;
    
    for (u = 1; u <= maxd - 1; u++) {
        v = (int)((cur.p1*(2*u-1)+cur.q1)/(2*cur.q1));
        x = centerx + dx*u - orientation*dy*v;
        y = centery + dy*u + orientation*dx*v;
        seenspace = seenblock = false;
        for (; ; v++) {
            if (tile[x][y].canpass(terrain)) {
                if (!seenspace) {
                    if (seenblock) {
                        // cut off bottom
                        cur.p1 = 2*v-1;
                        cur.q1 = 2*u-1;
                    }
                    seenspace = true;
                }
            }
            else {
                if (seenspace) {
                    // cut off top
                    cur.p2 = 2*v-1;
                    cur.q2 = 2*u+1;
                    break;
                }
                seenblock = true;
            }
            if ((2*v+1)*cur.q2 >= cur.p2*(2*u+1))
                break;
            x -= orientation*dy;
            y += orientation*dx;
        } // for v
        if (!seenspace || cur.p1*cur.q2 > cur.p2*cur.q1)
            return false;
    } // for u

    return true;
}

void Map::reveal(int x, int y) {
    tile[x][y].seen |= (TILEFLAG_SEEN | TILEFLAG_INLOS);
    display(x, y);
}

void Map::hide(int x, int y) {
    tile[x][y].seen &= TILEFLAG_SEEN;
    display(x, y);
}

void Map::revealterrain(int x, int y) {
    tile[x][y].seen |= TILEFLAG_SEEN;
    display(x, y);
}

void Map::setvalid(int x, int y) {
    tile[x][y].seen |= TILEFLAG_VALID;
}

void Map::resetvalid(int x, int y) {
    tile[x][y].seen &= ~TILEFLAG_VALID;
}

void Map::save(std::ostream & out) {
    int x, y;

    for (x = 0; x < MAPHEIGHT; x++) {
        for (y = 0; y < MAPWIDTH; y++) {
            out << tile[x][y].type << ' '
                << std::hex << (int)tile[x][y].seen << std::dec << ' '
                << tile[x][y].being << '\n';
        }
    }
}

void Map::load(std::istream & in) {
    int x, y, t, seen;

    for (x = 0; x < MAPHEIGHT; x++) {
        for (y = 0; y < MAPWIDTH; y++) {
            in >> t
               >> std::hex >> seen >> std::dec
               >> tile[x][y].being;
            tile[x][y].type = (TileType)t;
            tile[x][y].seen = seen;
        }
    }
}

int dist(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    if (dx > dy)
        return dx;
    else
        return dy;
}

char terrainsymbol(TerrainType terrain) {
    switch (terrain) {
    case TERRAIN_FLOOR:
        return '.';
    case TERRAIN_TUNNEL:
        return ':';
    case TERRAIN_CHASM:
        return '_';
    case TERRAIN_WALL:
        return '#';
    case TERRAIN_DARK:
        return '?';
    case TERRAIN_SHALLOW:
        return '-';
    case TERRAIN_DEEP:
        return '=';
    case TERRAIN_ROUGH:
        return '!';
    case TERRAIN_SUB:
        return '~';
    case TERRAIN_METAL:
        return '0';
    case TERRAIN_HIGH:
        return '*';
    default:
        return 'E';
    }
}
