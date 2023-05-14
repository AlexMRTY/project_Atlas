#ifndef collisionDetection_h
#define collisionDetection_h

#include "SDL2/SDL_mixer.h"
#include <stdbool.h>
#include "player.h"

#include "globalConst.h"
#include "player.h"
#include "world.h"
#include "coins.h"

#include <SDL2/SDL.h>

int collisionWithWall(int dx, int dy);
bool collision(int dx, int dy);
bool collisionWithPlayer(Player players[], int currentPlayer, int *nrOfPlayers, SDL_Rect *nextPos, Mix_Chunk *deathSound, UDPpacket *packet, UDPsocket *client_socket);
bool collisionWithCoins(Coins coins[], int *numberOfPoints, SDL_Rect *rect, int *update, int currentPlayer);
#endif