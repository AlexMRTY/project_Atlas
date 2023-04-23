#ifndef PLAYER_H
#define PLAYER_H
#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"
#include <stdbool.h>

typedef struct Player
{
    int id;
    SDL_Rect rect;
    int numberOfPoints;
    int movement;
    IPaddress address;
    int isAlive;
} Player;

Player initializePlayer(int *number_of_player, Player players[], UDPpacket *recieve);
void addToTheList(int id, int x, int y, int movement, int nrOfPoints, int *number_of_player, Player players[]);
void updatePlayer(int x, int y, int movement, int numberOfPoints, Player players[], int i);
void movePlayer(Player *playerr, int x, int y, int movement, int numberOfPoints);
#endif
