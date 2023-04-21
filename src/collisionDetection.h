#ifndef collisionDetection_h
#define collisionDetection_h
#include <stdbool.h>
#include <stdio.h>

#include "const.h"
#include "world.h"

bool collisionDetection(int dx, int dy);
bool collisionWithMap(int dx, int dy);
// collisionWithPlayer();
// outOfBoundsDetection();
#endif