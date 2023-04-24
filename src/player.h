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

Player initializePlayer(int *number_of_player, Player players[], UDPpacket *recieve);
void updatePlayerPos(Player players[], int index, int xCord, int yCord, int movemnt, int nrOfPoints);
Player addPlayer(int *id, int *x, int *y, int *nrOfPoints, int *movement, Player players[], int *number_of_player);
int playerExists(int *nrOfPlayers, Player players[], int *index, int lookUpId);
#endif
