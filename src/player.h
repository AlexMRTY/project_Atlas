#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include <stdbool.h>

typedef struct
{
    int id;
    SDL_Rect rect;
    int numberOfPoints;
    int movement;
    IPaddress address;
    int isAlive;
} Player;


// remove maybe
void initializePlayer();
bool isGameRunnin();
struct Player *getPlayer();
void movePlayer(int playerid, int x, int y);
void updateAllPlayers();
#endif
