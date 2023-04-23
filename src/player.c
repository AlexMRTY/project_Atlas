#include "player.h"
#include "globalConst.h"

void movePlayer(Player *playerr, int x, int y, int movement, int numberOfPoints)
{
    playerr->rect.x = x;
    playerr->rect.y = y;
    playerr->movement = movement;
    playerr->numberOfPoints = numberOfPoints;
}

Player initializePlayer(int *number_of_player, Player players[MAX_PLAYERS], UDPpacket *recieve)
{
    Player player = {(*number_of_player), {50, 50, PLAYER_WIDTH, PLAYER_HIGHT}, 0, 1, recieve->address};
    players[*number_of_player] = player;
    (*number_of_player)++;
    return player;
}

void addToTheList(int id, int x, int y, int movement, int nrOfPoints, int *number_of_player, Player players[MAX_PLAYERS])
{
    Player player = {id, {x, y, PLAYER_WIDTH, PLAYER_HIGHT}, nrOfPoints, movement};
    players[*number_of_player] = player;
    (*number_of_player)++;
}

void updatePlayer(int x, int y, int movement, int numberOfPoints, Player players[], int i)
{
    players[i].rect.x = x;
    players[i].rect.y = y;
    players[i].movement = movement;
    players[i].numberOfPoints = numberOfPoints;
}