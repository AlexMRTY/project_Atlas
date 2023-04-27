#ifndef COINS_H
#define COINS_H

#include "SDL2/SDL.h"

typedef struct
{
    SDL_Rect coin;
    int isVisible;
    int points;
    int id;
} Coins;

void initializeCoins(Coins coins[], int *numberOfCoins);
void updateCoins(Coins coins[], int j, int isvisible);
#endif