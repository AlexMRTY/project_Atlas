#include "collisionDetection.h"

bool collisionDetection(int dx, int dy) {
    int collison = 0;
    collison += collisionWithMap(dx, dy);
    collison += collisionWithMap(dx + (PLAYER_WIDTH - 1), dy);
    collison += collisionWithMap(dx, dy + (PLAYER_HIGHT - 1));
    collison += collisionWithMap(dx + (PLAYER_WIDTH - 1), dy + (PLAYER_HIGHT - 1));
    return collison > 0;
}
bool collisionWithMap(int dx, int dy) {
    // check the corners (all 4)
    // För långt till vänster och ner.
    int collumn = getCol(dx);
    int row = getRow(dy);
    printf(" X:%d Y:%d , Col:%d, Row:%d \n", dx, dy, collumn, row);

    if (getTileGrid(row, collumn) != 7) {
        // printf("%d %d\n", dx, dy);  // TEST
        return true;
    }
    return false;
}