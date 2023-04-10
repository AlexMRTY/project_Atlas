#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include <stdbool.h>

typedef struct
{
    int id;
    SDL_Rect rect;
    IPaddress address;
} Player;

void initializePlayer();
bool isGameRunnin();
struct Player *getPlayer();
void movePlayer(int playerid, int x, int y);
void updateAllPlayers();
#endif