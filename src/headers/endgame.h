#ifndef endgame_h
#define endgame_h

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globalConst.h"
#include <SDL2/SDL_ttf.h>

void endGame(SDL_Renderer *pRenderer, TTF_Font* font, int state);

#endif