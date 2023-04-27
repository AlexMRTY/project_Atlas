#ifndef collisionDetection_h
#define collisionDetection_h
#include <stdbool.h>
#include "player.h"

#include "globalConst.h"
#include "player.h"
#include "world.h"

#include <SDL2/SDL.h>

int collisionWithWall(int dx, int dy);
bool collision(int dx, int dy);
bool collisionWithPlayer(Player players[], int currentPlayer, int *nrOfPlayers, SDL_Rect *nextPos); // returns the player's nr in the array or -1 if no collision.
#endif