#include "headers/globalConst.h"
#include "headers/player.h"


Player initializePlayer(int *number_of_player, Player players[MAX_PLAYERS], UDPpacket *recieve, int choosenOne)
{
    Player player = {(*number_of_player), {(50 + ((*number_of_player) * 50)), 50, PLAYER_WIDTH, PLAYER_HIGHT}, 0, 1, 1, 0, recieve->address};
    player.isHunter = player.id == choosenOne ? 1 : 0;
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

void updateIsAlive(int id, int nrOfPlayers, Player players[])
{
    for (int i = 0; i < nrOfPlayers; i++)
    {
        if (players[i].id == id && players[i].isAlive == 1)
            players[i].isAlive = 0;
            // (*eliminatedPlayers)++;
            printf("new kill...\n");

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

Player addPlayer(int *id, int *x, int *y, int *nrOfPoints, int *movement, Player players[], int *number_of_player, int *isAlive, int *isHunter)
{
    Player player = {*id, {*x, *y, PLAYER_WIDTH, PLAYER_HIGHT}, *nrOfPoints, *movement, *isAlive, *isHunter};
    if (*isHunter){
        printf("player %d is Hunter\n", *id);
    }
    players[*number_of_player] = player;
    (*number_of_player)++;
    return player;
}

bool isMonster(int id)
{
    return id == 3 ? true : false;
}

int isGameOver(Player me)
{
    return me.isAlive == 0 ? 1 : 0;
}

int winnerIs(Player players[])
{
    int prevScore = 0;
    int winner;
    for (int i=0 ; i<MAX_PLAYERS ; i++)
    {
        if (players[i].numberOfPoints > prevScore)
        {
            prevScore = players[i].numberOfPoints;
            winner = i;
        }
    }

    return winner;
}