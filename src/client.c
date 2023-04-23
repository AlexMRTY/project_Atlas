#include "client.h"
#include "player.h"
#include "globalConst.h"







int joinAccept(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement)
{
    return sscanf((char *)recieve->data, "join_accept %d %d %d %d %d", x, y, id, nrOfPoints, movement);
}

int recievePlayerData(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement)
{
    return sscanf((char *)recieve->data, "player_data %d %d %d %d %d", x, y, id, nrOfPoints, movement);
}

Player addPlayer(int *id, int *x, int *y, int *nrOfPoints, int *movement, Player players[], int *number_of_player)
{
    Player player = {*id, {*x, *y, PLAYER_WIDTH, PLAYER_HIGHT}, *nrOfPoints, *movement};
    players[*number_of_player] = player;
    (*number_of_player)++;

    // printf("I am run\n");
    return player;
}

void printPlayerData(UDPpacket *recieve)
{
     // Print data on console
    printf("UDP Packet from server\n");
    printf("\tData:    %s\n", (char *)recieve->data);
    printf("\tAddress: %x %x\n", recieve->address.host, recieve->address.port);
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

// maybe put in player.c
void updatePlayerPos(Player players[],int index, int xCord, int yCord, int movemnt, int nrOfPoints)
{
    players[index].rect.x = xCord;
    players[index].rect.y = yCord;
    players[index].movement = movemnt;
    players[index].numberOfPoints = nrOfPoints;
}


void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket) 
{
    // Send player position update to server
    sprintf((char *)packet->data, "%d %d %d %d %d", me->rect.x, me->rect.y, me->id, me->numberOfPoints, me->movement);
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(*client_socket, -1, packet);
}

void HandleUDPRecv(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer)
{
    // *joinedServer = false;
    while (SDLNet_UDP_Recv(*client_socket, recieve))
    {
        // Prints the recieved player data to terminal.
        printPlayerData(recieve);

        // Temp
        int x, y, id, nrOfPoints, movement;
        int x2, y2, id2, nrOfPoints_2, movement_2;

        // If not joined yet, join. And read data.
        if (!(*joinedServer) && 
            joinAccept(recieve, &x, &y, &id, &nrOfPoints, &movement) == 5 && 
            *number_of_player <= MAX_PLAYERS)
        { 
            printf("Joined server!\n");
            *me = addPlayer(&id, &x, &y, &nrOfPoints, &movement, players, number_of_player);
            *joinedServer = true;

        }
        // If joined read player data.
        else if (recievePlayerData(recieve, &x2, &y2, &id2, &nrOfPoints_2, &movement_2) == 5)
        {
            int index = -1;
            if (*number_of_player <= MAX_PLAYERS)
            {
                // Look if the player already exists.
                int found = playerExists(number_of_player, players, &index, id2);
                
                // If not, add player
                if (!found)
                {
                    addPlayer(&id2, &x2, &y2, &nrOfPoints_2, &movement_2, players, number_of_player);
                    printf("Added player with ID %d\n", id2);
                } 
                // Else Update the player position on Window.
                else
                {
                    updatePlayerPos(players, index, x2, y2, movement_2, nrOfPoints_2);
                }
            }
        }
        // In case the recieved packet is not from our server. 
        else
        {
            printf("Unknown message received: %s\n", (char *)packet->data);
        }
    }
}