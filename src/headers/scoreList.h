#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globalConst.h"
#include <SDL2/SDL_ttf.h>

void ScoreListMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font);
#endif
