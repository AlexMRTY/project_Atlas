#ifndef events_h
#define events_h
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "SDL2/SDL_mixer.h"

#include "player.h"

void handleEvents(SDL_Rect *rect, int *movement, bool *quit, Mix_Chunk *music, Player players[], int currentPlayer, int *nrOfPlayers);
bool handleQuit(SDL_Event *event);
void transformCharacter(SDL_Event *event, SDL_Rect *rect, int *movement, Mix_Chunk *music, Player players[], int currentPlayer, int *nrOfPlayers);
#endif