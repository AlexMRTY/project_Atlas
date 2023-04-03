#ifndef PLAYER_H
#define PLAYER_H
#include "/usr/local/include/SDL2/SDL.h"
#include <stdbool.h>

typedef struct
{
    int playerId;
    SDL_Rect rect;
} Player;

void initializePlayer();
bool isGameRunnin();
struct Player *getPlayer();
void movePlayer(int playerid, int x, int y);

#endif