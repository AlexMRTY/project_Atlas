#ifndef render_h
#define render_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "coins.h"

void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);
void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]);
void renderPlayers(SDL_Renderer *pRenderer, SDL_Texture **pTexture, SDL_Rect *subtextures, int num_subtextures, Player *players, int num_players, Player me);
void loadCoins(SDL_Renderer *pRenderer, SDL_Texture **pTexture, Coins coins[], int *numCoins);
void renderCoins(SDL_Renderer *pRenderer, SDL_Texture **pTexture, Coins coins[], int numCoins);
#endif