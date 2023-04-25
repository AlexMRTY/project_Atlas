#include "collisionDetection.h"
#include "globalConst.h"
#include "world.h"

int collisionWithWall(int dx, int dy)
{

    int collison = 0;

    printf(" X:%d Y:%d \n", dx, dy);
    collison += collision(dx, dy);                               // 1
    collison += collision(dx + PLAYER_WIDTH, dy);                // 2
    collison += collision(dx, dy + PLAYER_HIGHT);                // 3
    collison += collision(dx + PLAYER_WIDTH, dy + PLAYER_HIGHT); // 4
    return collison;
}

bool collision(int dx, int dy)
{
    int collumn = getCol(dx);
    int row = getRow(dy);
    return getTileGrid(row, collumn) != 7 ? true : false;
}