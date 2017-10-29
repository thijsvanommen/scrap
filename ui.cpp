#include "ui.h"

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
		move(2, 0);
		refresh();
		while (getch() == ERR);
		return false;
	}

	if (!has_colors()) {
		clear();
		mvaddstr(0, 0, "Error: your console doesn't support colors.");
		move(1, 0);
		refresh();
		while (getch() == ERR);
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
	refresh();
}

void Interface::query(char * question, int fore) {
	flushmsg();
	msg(question, fore);
	refresh();
}

bool Interface::confirm(char * question, int fore) {
	int key;
	bool ans, valid;

	query(question);
	
	do {
		while ((key = getch()) == ERR);
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

int Interface::selectitem(char * question) {
	int ret;
	bool done = false;
	Command c;

	while (!done) {
		query(question);
		c = getcommand();
		switch (c) {
			case COMMAND_CANCEL: ret = -1; done = true; break;
			case COMMAND_1: ret = 0; done = true; break;
			case COMMAND_2: ret = 1; done = true; break;
			case COMMAND_3: ret = 2; done = true; break;
			case COMMAND_4: ret = 3; done = true; break;
			case COMMAND_5: ret = 4; done = true; break;
			case COMMAND_6: ret = 5; done = true; break;
			case COMMAND_7: ret = 6; done = true; break;
			case COMMAND_8: ret = 7; done = true; break;
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
	int key;
	int attrs = COLOR_PAIR((COLOR_WHITE ^ 7) + (COLOR_CYAN << 3));

	move(0, 73);
	attron(attrs);
	addstr(" More ");
	attroff(attrs);

	do {
		key = getch();
	} while (key == ERR);
	
	clearmsg();
}

Command Interface::getcommand() {
	int key;
	Command command;

	while ((key = getch()) == ERR);
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
		case 's':
			command = COMMAND_SWITCH; break;
		case '>':
			command = COMMAND_DOWN; break;
		case 27:
		case 'x':
			command = COMMAND_CANCEL; break;
		/* case KEY_TAB:
			command = COMMAND_NEXT; break; */ // FIXME
		case '?':
			command = COMMAND_HELP; break;
		case 'Q':
			command = COMMAND_QUIT; break;
	}

	return command;
}

void Interface::clearstatus() {
	for (int i = 0; i < STATUSHEIGHT; i++) {
		move(STATUSTOP + i, STATUSLEFT);
		clrtoeol();
	}
}

void Interface::clearitemslot(int slotno) {
	for (int i = 0; i < 3; i++) {
		move(STATUSTOP + 3*slotno + i, STATUSLEFT);
		clrtoeol();
	}
}

void Interface::displayitemslot(int slotno, char * name, int status,
				bool active) {
	int attrs = (active ? COLOR_PAIR(COLOR_CYAN ^ 7)  | A_BOLD
					: COLOR_PAIR(COLOR_WHITE ^ 7));
	clearitemslot(slotno);
	
	attron(attrs);
	mvaddch(STATUSTOP + 3*slotno, STATUSLEFT, '1'+slotno);
	move(STATUSTOP + 3*slotno, STATUSLEFT + 2);
	addstr(name);
	attroff(attrs);
	
	displayitembar(slotno, status, active);
}

void Interface::displayitembar(int slotno, int status, bool active) {
	int fore, back, length;
	int i;

	if (status <= 25)		back = COLOR_RED;
	else if (status <= 50)	back = COLOR_YELLOW;
	else if (status < 100)	back = COLOR_GREEN;
	else					back = COLOR_BLUE;

	if (active)		fore = COLOR_LWHITE;
	else			fore = COLOR_WHITE;
	
	length = STATUSWIDTH - (int)((100 - status) * STATUSWIDTH / 100);

	move(STATUSTOP + 3*slotno + 1, STATUSLEFT);
	for (i = 0; i < length; i++)
		bartext(i, status, fore, back);
	for (; i < 30; i++)
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

void Interface::displayhelp() {
	int attrs = COLOR_PAIR((COLOR_WHITE ^ 7) | (COLOR_BLUE << 3)) | A_BOLD;
	attron(attrs);
	mvaddstr(5, 9, "Controls:");
	attroff(attrs);
	attrs = COLOR_PAIR((COLOR_BLUE ^ 7) | (COLOR_BLUE << 3));
	attron(attrs);
	addstr("_____________________");
	attroff(attrs);
	windowline( 6, "movement:                 ");
	windowline( 7, "                          ");
	windowline( 8, "7 8 9      y k u          ");
	windowline( 9, " \\|/        \\|/           ");
	windowline(10, "4-@-6  or  h-@-l          ");
	windowline(11, " /|\\        /|\\           ");
	windowline(12, "1 2 3      b j n          ");
	windowline(13, "                          ");
	windowline(14, "1-8: use system 1-8       ");
	windowline(15, "  .: wait                 ");
	windowline(16, "  s: switch systems on/off");
	windowline(17, "  >: take exit            ");
	windowline(18, "  ?: display this page    ");
	windowline(19, "  Q: quit game            ");
	attron(attrs);
	mvaddstr(20, 9, "______________________________");
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
