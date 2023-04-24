#include "globalConst.h"
#include "player.h"

Player initializePlayer(int *number_of_player, Player players[MAX_PLAYERS], UDPpacket *recieve)
{
    Player player = {(*number_of_player), {50, 50, PLAYER_WIDTH, PLAYER_HIGHT}, 0, 1, recieve->address};
    players[*number_of_player] = player;
    (*number_of_player)++;
    return player;
}

void updatePlayerPos(Player players[], int index, int xCord, int yCord, int movemnt, int nrOfPoints)
{
    players[index].rect.x = xCord;
    players[index].rect.y = yCord;
    players[index].movement = movemnt;
    players[index].numberOfPoints = nrOfPoints;
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

Player addPlayer(int *id, int *x, int *y, int *nrOfPoints, int *movement, Player players[], int *number_of_player)
{
    Player player = {*id, {*x, *y, PLAYER_WIDTH, PLAYER_HIGHT}, *nrOfPoints, *movement};
    players[*number_of_player] = player;
    (*number_of_player)++;

    return player;
}