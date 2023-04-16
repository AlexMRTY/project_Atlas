#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include <stdbool.h>

typedef struct
{
    int id;
    SDL_Rect rect;
    IPaddress address;
    bool isAlive;
    int numberOfPoints;
    char pngName[50];
    int movement;
} Player;

void initializePlayer();
bool isGameRunnin();
struct Player *getPlayer();
void movePlayer(int playerid, int x, int y);
void updateAllPlayers();
#endif