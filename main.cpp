#include "main.h"

int main() {
    bool runprogram = true;

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

    GAME.init();
    GAME.state = GAMESTATE_RUNNING;
    do {
        switch (GAME.state) {
        case GAMESTATE_RUNNING:
            GAME.mainloop();
            break;

        case GAMESTATE_SAVE:
            if (GAME.savegame()) {
                UI.msg("Game saved successfully.");
                GAME.state = GAMESTATE_QUIT;
            }
            else {
                UI.msg("An error occured while trying to write the savefile!");
                GAME.state = GAMESTATE_RUNNING;
            }
            break;

        case GAMESTATE_LOAD:
            {
                Game restoredgame;
                if (restoredgame.loadgame()) {
                    UI.msg("Game restored successfully.");
                    GAME = restoredgame;
                    restoredgame.map = NULL;
                    GAME.startplay();
                }
                else
                    UI.msg(
                        "An error occured while trying to read the savefile!"
                        );
                GAME.state = GAMESTATE_RUNNING;
            }
            break;

        default:
            if (UI.confirm("Would you like to play again? (y/n)")) {
                GAME.init();
                GAME.state = GAMESTATE_RUNNING;
            }
            else
                runprogram = false;
            break;
        }
    } while (runprogram);
    
    UI.close();

    return 0;
}

