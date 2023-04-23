
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"

void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);
void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]);
void renderPlayers(SDL_Renderer *pRenderer, SDL_Texture **pTexture, SDL_Rect *subtextures, int num_subtextures, Player *players, int num_players, Player me);
