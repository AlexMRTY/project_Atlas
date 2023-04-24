#ifndef collisionDetection_h
#define collisionDetection_h
#include <stdbool.h>

#include "globalConst.h"
#include "player.h"
#include "world.h"
int collisionWithWall(int dx, int dy);
bool collision(int dx, int dy);
int collisionWithPlayer(Player players[], int currentPlayer, int nrOfPlayers);  // returns the player's nr in the array or -1 if no collision.
#endif