#ifndef START_H
#define START_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

int startMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font);
void activeMenu(SDL_Renderer *pRenderer, SDL_Surface *menus[], SDL_Color color[], SDL_Rect pos[], SDL_Texture *temp1, SDL_Texture *temp2, TTF_Font *font, int selected);

#endif