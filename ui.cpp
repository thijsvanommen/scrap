#include "being.h"
#include "item.h"
#include "game.h"
#include "ui.h" // needs to come last to avoid curses/std-conflict

extern Interface UI;

bool Interface::init() {
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);

#ifdef WIN32
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), 0); // hide mouse cursor
#endif

    if (LINES < 25 || COLS < 80) {
        clear();
        mvaddstr(0, 0, "Error: your console should be at least");
        mvaddstr(1, 0, "25x80 characters to run this game.");
        mvaddstr(3, 0, "Press Q to quit or any other key to try again.");
        move(4, 0);
        
        int key;
        while (true) {
            key = getkey();
            if (key == 'q' || key == 'Q')
                return false;
            if (LINES >= 25 && COLS >= 80)
                break;
        }
    }

    if (!has_colors()) {
        clear();
        mvaddstr(0, 0, "Error: your console doesn't support colors.");
        mvaddstr(2, 0, "Press any key to quit.");
        move(3, 0);
        getkey();
        return false;
    }

    start_color();
    for (int i = 1; i < 64; i++)
        init_pair(i, (i & 7) ^ 7, i >> 3);

    clear();
    refresh();

    msgdisplaylen = 0;

    return true;
}

void Interface::close() {
    refresh();
#ifdef DEBUG
    msg("The application is about to exit.", COLOR_LWHITE);
    moreprompt();
#endif
    delwin(stdscr);
    endwin();
}

void Interface::display(int row, int column, char data, int fore, int back) {
    int attrs = COLOR_PAIR(((fore & 7) ^ 7) + (back << 3));
    if (fore & 8)
        attrs = attrs | A_BOLD;
    attron(attrs);
    mvaddch(row, column, data);
    attroff(attrs);
}

int Interface::getkey() {
    int key;
    
    refresh();
    key = getch();
    
    return key;
}

void Interface::clearscreen() {
    for (int i = DISPLAYTOP; i < LINES; i++) {
        move(i, 0);
        clrtoeol();
    }
}

void Interface::msg(char * msg, int fore) {
    // TODO: save msg history

    int totallen = strlen(msg);
    int from = 0, printlen;
    int attrs = COLOR_PAIR((fore & 7) ^ 7);
    if (fore & 8)
        attrs = attrs | A_BOLD;

    while (true) {
        printlen = MAXMSGLEN - msgdisplaylen;
        if (printlen >= totallen - from)
            // message length is limiting factor
            printlen = totallen - from;
        else {
            // screen space is limiting factor
            while (printlen >= 0 && msg[from + printlen] != ' ')
                printlen--;
            // TODO: add safeguard against long words
        }
        
        if (printlen > 0) {
            attron(attrs);
            move(0, msgdisplaylen);
            addnstr(&msg[from], printlen);
            attroff(attrs);
        }
        else
            printlen = 0;

        if (from + printlen >= totallen)
            break;

        from += printlen;
        if (msg[from] == ' ')
            from++;
        moreprompt();
    };

    msgdisplaylen += printlen + 1;

#ifdef DEBUG
    refresh();
#endif
}

void Interface::query(char * question, int fore) {
    flushmsg();
    msg(question, fore);
}

bool Interface::confirm(char * question, int fore) {
    int key;
    bool ans, valid;

    query(question);
    
    do {
        key = getkey();
        valid = true;
        switch (key) {
            case 'y':
            case 'Y':
                ans = true; break;
            case 'n':
            case 'N':
                ans = false; break;
            default:
                valid = false;
        }
    } while (!valid);
    clearmsg();

    return ans;    
}

int Interface::selectitem(char * question, Being & viewed) {
    int ret;
    bool done = false;
    bool readcommand = true;
    Command c;

    while (!done) {
        if (readcommand) {
            query(question);
            c = getcommand();
        }
        else
            readcommand = true;
        switch (c) {
        case COMMAND_CANCEL:
            ret = -1; done = true; break;
        case COMMAND_1:
            ret = 0; done = true; break;
        case COMMAND_2:
            ret = 1; done = true; break;
        case COMMAND_3:
            ret = 2; done = true; break;
        case COMMAND_4:
            ret = 3; done = true; break;
        case COMMAND_5:
            ret = 4; done = true; break;
        case COMMAND_6:
            ret = 5; done = true; break;
        case COMMAND_7:
            ret = 6; done = true; break;
        case COMMAND_8:
            ret = 7; done = true; break;
        case COMMAND_DETAILS:
            c = detailspage(viewed);
            GAME.normaldisplay();
            viewed.displaystatus();
            readcommand = false;
            break;;
        default:
            break;
        }
    }

    return ret;
}

void Interface::flushmsg() {
    if (msgdisplaylen > 0)
        moreprompt();
};

void Interface::clearmsg() {
    move(0, 0);
    clrtoeol();
    msgdisplaylen = 0;
}

void Interface::moreprompt() {
    int attrs = COLOR_PAIR((COLOR_WHITE ^ 7) + (COLOR_CYAN << 3));

    move(0, 73);
    attron(attrs);
    addstr(" More ");
    attroff(attrs);

    getkey();
    
    clearmsg();
}

Command Interface::getcommand() {
    int key;
    Command command;

    key = getkey();
    clearmsg();
    switch (key) {
        case KEY_UP:
#ifdef WIN32
        case KEY_A2:
#endif
        case 'k':
            command = COMMAND_NORTH; break;
        case KEY_RIGHT:
#ifdef WIN32
        case KEY_B3:
#endif
        case 'l':
            command = COMMAND_EAST; break;
        case KEY_DOWN:
#ifdef WIN32
        case KEY_C2:
#endif
        case 'j':
            command = COMMAND_SOUTH; break;
        case KEY_LEFT:
#ifdef WIN32
        case KEY_B1:
#endif
        case 'h':
            command = COMMAND_WEST; break;
        case KEY_HOME:
        case KEY_A1:
        case 'y':
            command = COMMAND_NW; break;
        case KEY_END:
        case KEY_LL:
        case KEY_C1:
        case 'b':
            command = COMMAND_SW; break;
        case KEY_PPAGE:
        case KEY_A3:
        case 'u':
            command = COMMAND_NE; break;
        case KEY_NPAGE:
        case KEY_C3:
        case 'n':
            command = COMMAND_SE; break;
        case '.':
#ifdef WIN32
    case PADSTOP: // . on numpad
    case KEY_B2:
#endif
            command = COMMAND_WAIT; break;
        case '1':
            command = COMMAND_1; break;
        case '2':
            command = COMMAND_2; break;
        case '3':
            command = COMMAND_3; break;
        case '4':
            command = COMMAND_4; break;
        case '5':
            command = COMMAND_5; break;
        case '6':
            command = COMMAND_6; break;
        case '7':
            command = COMMAND_7; break;
        case '8':
            command = COMMAND_8; break;
    case 'd':
        command =  COMMAND_DETAILS; break;
    case '@':
        command = COMMAND_LOCATION; break;
    case 'r':
        command = COMMAND_REORDER; break;
        case 's':
            command = COMMAND_SWITCH; break;
        case '>':
            command = COMMAND_DOWN; break;
        case 27:
        case 'x':
            command = COMMAND_CANCEL; break;
    case '+':
#ifdef WIN32
        case PADPLUS:
#endif
            command = COMMAND_NEXT; break;
    case '-':
#ifdef WIN32
        case PADMINUS:
#endif
        command = COMMAND_PREV; break;
        case '?':
            command = COMMAND_HELP; break;
    case 'S':
        command = COMMAND_SAVE; break;
    case 'R':
        command = COMMAND_LOAD; break;
        case 'Q':
            command = COMMAND_QUIT; break;
    default:
        command = COMMAND_UNDEFINED; break;
    }

    return command;
}

void Interface::clearstatus() {
    for (int i = 0; i < STATUSHEIGHT; i++) {
        move(STATUSTOP + i, STATUSLEFT);
        clrtoeol();
    }
}

void Interface::clearitemslot(int slotno, int left) {
    for (int i = 0; i < 3; i++) {
        move(STATUSTOP + 3*slotno + i, left);
        clrtoeol();
    }
}

void Interface::displayitemslot(int slotno, Item & it, int left) {
    int attrs = (it.active != ACTIVE_OFF ? COLOR_PAIR(COLOR_CYAN ^ 7) | A_BOLD
                 : COLOR_PAIR(COLOR_WHITE ^ 7));
    clearitemslot(slotno, left);
    
    attron(attrs);
    mvaddch(STATUSTOP + 3*slotno, left, '1'+slotno);
    move(STATUSTOP + 3*slotno, left + 2);
    addstr(it.getname());
    attroff(attrs);
    
    move(STATUSTOP + 3*slotno + 1, left);
    displayitembar(slotno, it.status, it.active != ACTIVE_OFF);
}

void Interface::displayitembar(int slotno, int status, bool active) {
    int fore, back, length;
    int i;

    if (status <= 25)        back = COLOR_RED;
    else if (status <= 50)    back = COLOR_YELLOW;
    else if (status < 100)    back = COLOR_GREEN;
    else                    back = COLOR_BLUE;

    if (active)        fore = COLOR_LWHITE;
    else            fore = COLOR_WHITE;
    
    length = STATUSWIDTH - (int)((100 - status) * STATUSWIDTH / 100);

    for (i = 0; i < length; i++)
        bartext(i, status, fore, back);
    for (; i < STATUSWIDTH; i++)
        bartext(i, status, fore, COLOR_BLACK);
}

void Interface::bartext(int i, int status, int fore, int back) {
    const int mid = STATUSWIDTH / 2;
    char symbol = ' ';
    int attrs;
    
    switch (i) {
        case mid - 2:
            if (status == 100)
                symbol = '1';
            break;
        case mid - 1:
            if (status >= 10)
                symbol = '0' + (status / 10) % 10;
            break;
        case mid:
            symbol = '0' + status % 10;
            break;
        case mid + 1:
            symbol = '%';
            break;
    }
    
    if (symbol == ' ') {
        // some consoles may skip spaces, so this forces the background
        // color to be displayed
        fore = back;
        symbol = '_';
    }
    attrs = COLOR_PAIR(((fore & 7) ^ 7) + (back << 3));
    if (fore & 8)
        attrs |= A_BOLD;
    attron(attrs);
    addch(symbol);
    attroff(attrs);
}

Command Interface::detailspage(Being & b) {
    Command c;
    bool viewingplayer = false;

    do {
        Being & viewed = (viewingplayer ? GAME.being[0] : b);
        displaydetails(viewed);
        if (viewed.isplayer) {
            if (b.isplayer)
                msg("Viewing your system details. Press any key...");
            else {
                char msgbuf[128];

                strcpy(msgbuf, "\
Viewing your system details. Press d to view ");
                strcat(msgbuf, b.getname());
                strcat(msgbuf, "'s systems.");
                msg(msgbuf);
            }
        }
        else {
            char msgbuf[128];

            strcpy(msgbuf, "Viewing ");
            strcat(msgbuf, viewed.getname());
            strcat(msgbuf, "'s system details. Press d to view your systems.");
            msg(msgbuf);
        }
        c = getcommand();
        if (c == COMMAND_DETAILS)
            if (b.isplayer)
                c = COMMAND_UNDEFINED;
            else
                viewingplayer = !viewingplayer;
    } while (c == COMMAND_DETAILS);

    return c;
}

void Interface::displaydetails(Being & b) {
    int attrs, top, left;

    for (int slotno = 0; slotno < EQSIZE; slotno++) {
        Item & it = b.eq[slotno];
        if (it.gettype() != ITEM_VOID) {
            displayitemslot(slotno, it, 0);

            attrs = COLOR_PAIR(COLOR_WHITE ^ 7);
            top = STATUSTOP + 3*slotno;
            left = STATUSWIDTH + 2;
            attron(attrs);
            move(top, left);
            switch(it.gettype()) {
            case ITEM_POWER:
                attron(A_BOLD);
                addstr("Power source");
                attroff(A_BOLD);
                mvaddstr(top, left + 14, "Power ");
                attron(A_BOLD);
                printw("%d", it.prop[POWER_ANY]);
                attroff(A_BOLD);
                break;
            case ITEM_WEAPON:
                attron(A_BOLD);
                addstr("Weapon");
                attroff(A_BOLD);

                mvaddstr(top, left + 14, "Strength ");
                attron(A_BOLD);
                printw("%d", it.prop[WEAPON_POWER]);
                attroff(A_BOLD);

                move(top, left + 26);
                if (it.prop[WEAPON_DAMAGETYPE] != DAMAGE_NORMAL) {
                    addstr("Anti-");
                    displaydamagetypelist(it.prop[WEAPON_DAMAGETYPE]);
                }
                else {
                    addstr("Does normal damage");
                }

                mvaddstr(top + 1, left + 14, "Range ");
                attron(A_BOLD);
                printw("%d", it.prop[WEAPON_RANGE]);
                attroff(A_BOLD);

                mvaddstr(top + 1, left + 26, "Terrain ");
                attroff(attrs);
                displayterrainflags(it.prop[WEAPON_TERRAIN]);
                attron(attrs);
                break;
            case ITEM_PROPULSION:
                attron(A_BOLD);
                addstr("Propulsion");
                attroff(A_BOLD);

                mvaddstr(top, left + 14, "Speed ");
                attron(A_BOLD);
                printw("%d", it.prop[PROPULSION_SPEED]);
                attroff(A_BOLD);

                mvaddstr(top, left + 26, "Terrain ");
                attroff(attrs);
                displayterrainflags(it.prop[PROPULSION_TERRAIN]);
                attron(attrs);
                break;
            case ITEM_SENSOR:
                attron(A_BOLD);
                addstr("Sensor");
                attroff(A_BOLD);

                mvaddstr(top, left + 14, "Range ");
                attron(A_BOLD);
                printw("%d", it.prop[SENSOR_RANGE]);
                attroff(A_BOLD);

                mvaddstr(top, left + 26, "Terrain ");
                attroff(attrs);
                displayterrainflags(it.prop[SENSOR_TERRAIN]);
                attron(attrs);
                break;
            case ITEM_ARMOR:
                attron(A_BOLD);
                addstr("Armor");
                attroff(A_BOLD);

                mvaddstr(top, left + 14, "Strength ");
                attron(A_BOLD);
                printw("%d", it.prop[ARMOR_STRENGTH]);
                attroff(A_BOLD);

                if (it.prop[ARMOR_RESISTANCE]) {
                    mvaddstr(top, left + 26, "Resists ");
                    displaydamagetypelist(it.prop[ARMOR_RESISTANCE],
                                          top + 1, left + 14);
                }
                break;
            default:
                attron(A_BOLD);
                addstr("Miscellaneous item");
                attroff(A_BOLD);
                break;
            }
            if (it.energyuse >= 0) {
                mvaddstr(top + 1, left, "Power use ");
                attron(A_BOLD);
                printw("%d", it.energyuse);
                attroff(A_BOLD);
            }
            attroff(attrs);
        }
        else
            clearitemslot(slotno, 0);
    }
}

void Interface::displayterrainflags(int flags) {
    for (int terrain = 1; terrain < TERRAIN_WILDCARD; terrain <<= 1) {
        int attrs = (terrain & flags)
            ? COLOR_PAIR((COLOR_WHITE ^ 7) | (COLOR_BLUE << 3)) | A_BOLD
            : COLOR_PAIR(COLOR_BLACK ^ 7) | A_BOLD;
        attron(attrs);
        addch(terrainsymbol((TerrainType)terrain));
        attroff(attrs);
    }
}

void Interface::displaydamagetypelist(int flags, int newtop, int newleft) {
    int count = 0;

    displaydamagetype(flags, count, newtop, newleft, DAMAGE_NORMAL,
                      "normal damage");
    displaydamagetype(flags, count, newtop, newleft, DAMAGE_POWER,
                      "power source");
    displaydamagetype(flags, count, newtop, newleft, DAMAGE_WEAPON,
                      "weapon");
    displaydamagetype(flags, count, newtop, newleft, DAMAGE_PROPULSION,
                      "propulsion");
    displaydamagetype(flags, count, newtop, newleft, DAMAGE_SENSOR,
                      "sensor");
    displaydamagetype(flags, count, newtop, newleft, DAMAGE_ARMOR,
                      "armor");
    displaydamagetype(flags, count, newtop, newleft, DAMAGE_OTHER,
                      "other");
}

void Interface::displaydamagetype(int flags, int & count,
                                  int newtop, int newleft,
                                  int type, char * type_name) {
    if (count >= 1 && newleft < 0)
        return;
    if (flags & type) {
        count++;
        if (count > 1) {
            if (count == 2) {
                addstr(",");
                if (newleft < 0)
                    return;
                move(newtop, newleft);
            }
            else
                addstr(",");
        }
        attron(A_BOLD);
        addstr(type_name);
        attroff(A_BOLD);
    }
}

void Interface::displayhelp() {
    int attrs = COLOR_PAIR((COLOR_WHITE ^ 7) | (COLOR_BLUE << 3)) | A_BOLD;
    attron(attrs);
    mvaddstr(2, 9, "Controls:");
    attroff(attrs);
    attrs = COLOR_PAIR((COLOR_BLUE ^ 7) | (COLOR_BLUE << 3));
    attron(attrs);
    addstr("_____________________");
    attroff(attrs);
    windowline( 3, "movement:                 ");
    windowline( 4, "                          ");
    windowline( 5, "7 8 9      y k u          ");
    windowline( 6, " \\|/        \\|/           ");
    windowline( 7, "4-@-6  or  h-@-l          ");
    windowline( 8, " /|\\        /|\\           ");
    windowline( 9, "1 2 3      b j n          ");
    windowline(10, "                          ");
    windowline(11, "1-8: use system 1-8       ");
    windowline(12, "  .: wait                 ");
    windowline(13, "  d: view system details  ");
    windowline(14, "  @: display name of level");
    windowline(15, "  r: reorder systems      ");
    windowline(16, "  s: switch systems on/off");
    windowline(17, "  >: take exit            ");
    windowline(18, "  ?: display this page    ");
    windowline(19, "  S: save game            ");
    windowline(20, "  R: restore saved game   ");
    windowline(21, "  Q: quit game            ");
    attron(attrs);
    mvaddstr(22, 9, "______________________________");
    attroff(attrs);
    
    msg("Press any key...");
    moreprompt();
}

void Interface::windowline(int x, char * line) {
    int attrs = COLOR_PAIR((COLOR_BLUE ^ 7) | (COLOR_BLUE << 3));
    attron(attrs);
    mvaddstr(x, 9, "__");
    attroff(attrs);
    addstr(line);
    attron(attrs);
    addstr("__");
    attroff(attrs);
}

char * intcat(char * buf, unsigned int val) {
    char digit[2];
    
    if (val >= 10)
        intcat(buf, val / 10);
    digit[0] = '0' + (val % 10);
    digit[1] = '\0';
    strcat(buf, digit);
    return buf;
}

// napms(int ms): sleep for ms ms

/*
void colortest() {
    int i, j;
    for (i = 0; i < 16; i++)
        for (j = 0; j < 8; j++)
            UI.display(i, j, '#', i, j);
            // map.display(i, j);
}
*/
