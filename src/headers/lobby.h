#ifndef lobby_h
#define lobby_h

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globalConst.h"
#include <SDL2/SDL_ttf.h>

void waitingForPlayers(SDL_Renderer *pRenderer, TTF_Font* font, int numOfPlayers);

#endif