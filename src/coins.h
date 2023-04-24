#ifndef COINS_H
#define COINS_H

#include "SDL2/SDL.h"

typedef struct
{
    SDL_Rect coin;
    int isVisible;
    int points;
} Coins;

#endif