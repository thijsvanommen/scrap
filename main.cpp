#include "main.h"

int main() {
	if (!UI.init())
		return 1;

	seedrng();

    {
        char msgbuf[128];

        strcpy(msgbuf, "Welcome to ");
        strcat(msgbuf, SCRAP_VERSION);
        strcat(msgbuf, "! Press '?' for help.");
        UI.msg(msgbuf);
    }

	do {
        GAME.init();
        GAME.mainloop();
    } while (UI.confirm("Would you like to play again? (y/n)"));
	
	UI.close();

	return 0;
}

