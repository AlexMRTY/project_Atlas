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
    int isAlive;
    int isHunter;
    IPaddress address;
} Player;

Player initializePlayer(int *number_of_player, Player players[], UDPpacket *recieve, int choosenOne);
void updatePlayerPos(Player players[], int index, int xCord, int yCord, int movemnt, int nrOfPoints, int isAlive);
void updateMe(int isAlive, Player *me);
void updateIsAlive(int id, int nrOfPlayers, Player players[]);
Player addPlayer(int *id, int *x, int *y, int *nrOfPoints, int *movement, Player players[], int *number_of_player, int *isAlive, int *isHunter);
int playerExists(int *nrOfPlayers, Player players[], int *index, int lookUpId);
bool isMonster(int id);
int isGameOver(Player me);
#endif
