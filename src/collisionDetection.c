#include "headers/collisionDetection.h"
#include "headers/globalConst.h"
#include "headers/world.h"
#include "headers/client.h"
#include "headers/player.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

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
    return getTileGrid(row, collumn) != 7;
}

bool collisionWithPlayer(Player players[], int currentPlayer, int *nrOfPlayers, SDL_Rect *nextPos, Mix_Chunk *deathSound)
{
    for (int i = 0; i < (*nrOfPlayers); i++)
    {
        if (players[i].id != currentPlayer)
        {
            if (SDL_HasIntersection(nextPos, &players[i].rect))
            {

                if (isMonster(currentPlayer))
                {
                    players[i].isAlive = 0;
                    Mix_PlayChannel(-1, deathSound, 0);
                }

                return true;
            }
        }
    }
    return false;
}

bool collisionWithCoins(Coins coins[], int *numberOfPoints, SDL_Rect *rect, int *update, int currentPlayer)
{
    if (!isMonster(currentPlayer))
    {
        for (int i = 0; i < MAX_COINS; i++)
        {
            Coins *coin = &coins[i];
            if (SDL_HasIntersection(&coin->coin, rect))
            {
                if (coin->isVisible == 1)
                {
                    coin->isVisible = 0;
                    (*numberOfPoints) += coin->points;
                    *update = coin->id;
                    return true;
                }
            }
        }
    }
    return false;
}