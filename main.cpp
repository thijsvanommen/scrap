/*
 * Scrap, the Roguelike
 * Copyright 2005, 2006 Thijs van Ommen
 *
 * This file is part of Scrap, the Roguelike.
 *
 * Scrap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Scrap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scrap.  If not, see <http://www.gnu.org/licenses/>.
 */

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

