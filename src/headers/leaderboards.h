#ifndef LEADERBOARDS_H
#define LEADERBOARDS_H

#include "player.h"

void displayLeaderboard(SDL_Renderer * pRenderer, int * gameState, bool * quit, TTF_Font * fonts, Player players[], int *gameOver);


#endif