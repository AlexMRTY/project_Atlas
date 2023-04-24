#include "collisionDetection.h"

#include "globalConst.h"
#include "player.h"
#include "world.h"

int collisionWithWall(int dx, int dy) {
    // check the corners (all 4)
    //  1******2
    //  ********
    //  ********
    //  3******4
    int collison = 0;

    printf(" X:%d Y:%d \n", dx, dy);
    collison += collision(dx, dy);                                // 1
    collison += collision(dx + PLAYER_WIDTH, dy);                 // 2
    collison += collision(dx, dy + PLAYER_HIGHT);                 // 3
    collison += collision(dx + PLAYER_WIDTH, dy + PLAYER_HIGHT);  // 4
    return collison;
}

bool collision(int dx, int dy) {
    int collumn = getCol(dx);
    int row = getRow(dy);
    // printf(" X:%d Y:%d , Col:%d, Row:%d \n", dx, dy, collumn, row);

    // if (getTileGrid(row, collumn) != 7)
    // {
    //     return true;
    // }
    return getTileGrid(row, collumn) != 7 ? true : false;
}
int collisionWithPlayer(Player players[], int currentPlayer, int nrOfPlayers) {
    for (int i = 0; i < nrOfPlayers; i++) {
        if (i != currentPlayer) {
            if (players[i].rect.x == players[currentPlayer].rect.x && players[i].rect.y == players[currentPlayer].rect.y) {
                return i;
            }
        }
    }
    return -1;
}