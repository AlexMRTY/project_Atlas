#ifndef START_H
#define START_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

void startMenu(SDL_Renderer *pRenderer, int *escapePressed, bool *quit, TTF_Font *font);
void startTheGame();
void exitTheGame();
#endif