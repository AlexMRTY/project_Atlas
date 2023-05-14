#ifndef pause_h
#define pause_h

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "globalConst.h"
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL_mixer.h"

void pauseMenu(SDL_Renderer *pRenderer, int *escapePressed, bool *quit, TTF_Font *font, Mix_Chunk *keyPressed, Mix_Chunk *selectSound);
void activeOption(SDL_Renderer *pRenderer, SDL_Surface *menus[], SDL_Color color[], SDL_Rect pos[], SDL_Texture *temp1, SDL_Texture *temp2, TTF_Font *font, int selected);
#endif
