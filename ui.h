#ifndef UI_H
#define UI_H

#ifdef WIN32

#include "D:\Programs\Microsoft Visual Studio\VC98\Lib\pdcurs26\curses.h" // FIXME directory-independent
// #define MOUSE_MOVED // FIXME
#include <windows.h>

#else

#include <curses.h>
#include <ncurses.h>
#include <string>

#endif

const int MAXMSGLEN = 72;
const int DISPLAYTOP = 1;
const int DISPLAYLEFT = 0;
const int DISPLAYWIDTH = 48;
const int DISPLAYHEIGHT = 24;
const int STATUSTOP = 1;
const int STATUSLEFT = 50;
const int STATUSWIDTH = 30;
const int STATUSHEIGHT = 24;

const int COLOR_LBLACK	= COLOR_BLACK | 8;
const int COLOR_LBLUE	= COLOR_BLUE | 8;
const int COLOR_LGREEN	= COLOR_GREEN | 8;
const int COLOR_LCYAN	= COLOR_CYAN | 8;
const int COLOR_LRED	= COLOR_RED | 8;
const int COLOR_LMAGENTA	= COLOR_MAGENTA | 8;
const int COLOR_LYELLOW	= COLOR_YELLOW | 8;
const int COLOR_LWHITE	= COLOR_WHITE | 8;

enum Command {
	COMMAND_UNDEFINED,
	COMMAND_NORTH, COMMAND_NE, COMMAND_EAST, COMMAND_SE,
	COMMAND_SOUTH, COMMAND_SW, COMMAND_WEST, COMMAND_NW,
	COMMAND_WAIT,
	COMMAND_1, COMMAND_2, COMMAND_3, COMMAND_4,
	COMMAND_5, COMMAND_6, COMMAND_7, COMMAND_8,
	COMMAND_SWITCH, COMMAND_DOWN,
	COMMAND_CANCEL, COMMAND_NEXT, COMMAND_HELP, COMMAND_QUIT,
// AI only
	COMMAND_MISS
};

class Interface {
public:
	bool init();
	void close();
	void display(int row, int column, char data = ' ',
			int fore = COLOR_WHITE, int back = COLOR_BLACK);
	void msg(char * msg, int fore = COLOR_WHITE);
	void query(char * question, int fore = COLOR_LWHITE);
	bool confirm(char * question, int fore = COLOR_LWHITE);
	int selectitem(char * question);
	void flushmsg();
	void moreprompt();
	Command getcommand();
	void clearstatus();
	void clearitemslot(int slotno);
	void displayitemslot(int slotno, char * name, int status,
					bool active);
	void displayitembar(int slotno, int status, bool active);
	void displayhelp();

private:
	void clearmsg();
	void bartext(int i, int status, int fore, int back);
	void windowline(int x, char * line);

	int msgdisplaylen;
};

char * intcat(char * buf, unsigned int val);

#endif
