#ifndef START_H
#define START_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"

#include <stdbool.h>

void displayStartMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font, int *gameState, Mix_Chunk *clickOptionSFX, Mix_Chunk *selectSFX);
void activeMenu(SDL_Renderer *pRenderer, SDL_Surface *menus[], SDL_Color color[], SDL_Rect pos[], SDL_Texture *temp1, SDL_Texture *temp2, TTF_Font *font, int selected);
void startMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font, int *gameState, bool *joinedServer, UDPpacket *packet, UDPsocket *client_socket, Mix_Chunk *clickOptionSFX, Mix_Chunk *selectSFX);
#endif