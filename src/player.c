#include "globalConst.h"
#include "player.h"

Player initializePlayer(int *number_of_player, Player players[MAX_PLAYERS], UDPpacket *recieve)
{
    Player player = {(*number_of_player), {(50 + ((*number_of_player) * 50)), 50, PLAYER_WIDTH, PLAYER_HIGHT}, 0, 1, 1, recieve->address};
    players[*number_of_player] = player;
    (*number_of_player)++;
    return player;
}

void updatePlayerPos(Player players[], int index, int xCord, int yCord, int movemnt, int nrOfPoints, int isAlive)
{
    players[index].rect.x = xCord;
    players[index].rect.y = yCord;
    players[index].movement = movemnt;
    players[index].numberOfPoints = nrOfPoints;
    players[index].isAlive = isAlive;
}

void updateMe(int isAlive, Player *me)
{
    me->isAlive = isAlive;
}

void updateIsAlive(int id, int isAlive, int nrOfPlayers, Player players[])
{
    for (int i = 0; i < nrOfPlayers; i++)
    {
        if (players[i].id == id)
            players[i].isAlive = isAlive;
    }
}

int playerExists(int *nrOfPlayers, Player players[], int *index, int lookUpId)
{
    for (int i = 0; i < *nrOfPlayers; i++)
    {
        if (players[i].id == lookUpId)
        {
            *index = i;
            return 1;
        }
    }
    return 0;
}

Player addPlayer(int *id, int *x, int *y, int *nrOfPoints, int *movement, Player players[], int *number_of_player, int *isAlive)
{
    Player player = {*id, {*x, *y, PLAYER_WIDTH, PLAYER_HIGHT}, *nrOfPoints, *movement, *isAlive};
    players[*number_of_player] = player;
    (*number_of_player)++;
    return player;
}

bool isMonster(int id)
{
    // test
    if (id == 1)
        return false;
    else
        return true;
    /*
    if (id <= 2)
        return false;
    else
        return true;
        */
}

bool isGameOver(Player me)
{
    if (me.isAlive == 0)
        return true;
    else
        return false;
}