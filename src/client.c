#include "headers/client.h"
#include "headers/player.h"
#include "headers/globalConst.h"
#include "headers/coins.h"

int joinAccept(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement, int *isAlive, int *isHunter)
{
    return sscanf((char *)recieve->data, "join_accept %d %d %d %d %d %d %d", x, y, id, nrOfPoints, movement, isAlive, isHunter);
}

int recievePlayerData(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement, int *isAlive, int *isHunter)
{
    return sscanf((char *)recieve->data, "player_data %d %d %d %d %d %d %d", x, y, id, nrOfPoints, movement, isAlive, isHunter);
}

void printPlayerData(UDPpacket *recieve)
{
    // Print data on console
    printf("UDP Packet from server\n");
    printf("\tData:    %s\n", (char *)recieve->data);
    printf("\tAddress: %x %x\n", recieve->address.host, recieve->address.port);
}

void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket)
{
    sprintf((char *)packet->data, "%d %d %d %d %d %d", me->rect.x, me->rect.y, me->id, me->numberOfPoints, me->movement, me->isAlive);
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(*client_socket, -1, packet);
    // printf("Sending Coins Data\n");
}

void transmitCoins(Coins coins[], int numberOfCoins, UDPpacket *packet, UDPsocket *client_socket, int update)
{
    sprintf((char *)packet->data, "coins_update %d %d %d %d %d", coins[update].coin.x, coins[update].coin.y, coins[update].isVisible, coins[update].points, coins[update].id);
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(*client_socket, -1, packet);
}

void transmittDiedPlayer(UDPpacket *packet, UDPsocket *client_socket, int id, int numberOfPlayers)
{
    sprintf((char *)packet->data, "new kill %d", id);
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(*client_socket, -1, packet);
    // for (int i = 0; i < numberOfPlayers; i++)
    // {
    //     if (player[i].isAlive == 0)
    //     {
    //         sprintf((char *)packet->data, "new kill %d %d", player[i].id, player[i].isAlive);
    //         packet->len = strlen((char *)packet->data) + 1;
    //         SDLNet_UDP_Send(*client_socket, -1, packet);
    //     }
    // }
}

void HandleUDPRecv(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numberOfCoins)
{
    // *joinedServer = false;
    while (SDLNet_UDP_Recv(*client_socket, recieve))
    {
        // Prints the recieved player data to terminal.
        // printPlayerData(recieve);

        // Temp
        int x, y, id, nrOfPoints, movement, isAlive, coinX, coinY, isVisible, points, coinId, isHunter;
        int x2, y2, id2, nrOfPoints_2, movement_2, isAlive_2, isHunter_2;

        // If not joined yet, join. And read data.
        if (!(*joinedServer) &&
            joinAccept(recieve, &x, &y, &id, &nrOfPoints, &movement, &isAlive, &isHunter) == 7 &&
            *number_of_player <= MAX_PLAYERS)
        {
            printf("Joined server!\n");
            *me = addPlayer(&id, &x, &y, &nrOfPoints, &movement, players, number_of_player, &isAlive, &isHunter);
            *joinedServer = true;
        }
        // If joined read player data.
        else if (recievePlayerData(recieve, &x2, &y2, &id2, &nrOfPoints_2, &movement_2, &isAlive_2, &isHunter_2) == 7)
        {
            int index = -1;
            if (*number_of_player <= MAX_PLAYERS)
            {
                // Look if the player already exists.
                int found = playerExists(number_of_player, players, &index, id2);

                // If not, add player
                if (!found)
                {
                    addPlayer(&id2, &x2, &y2, &nrOfPoints_2, &movement_2, players, number_of_player, &isAlive_2, &isHunter_2);
                    printf("Added player with ID %d\n", id2);
                }
                else if (id2 == me->id)
                {
                    updateMe(isAlive_2, me);
                    updatePlayerPos(players, index, x2, y2, movement_2, nrOfPoints_2, isAlive_2);
                }
                // Else Update the player position on Window.
                else
                {
                    updatePlayerPos(players, index, x2, y2, movement_2, nrOfPoints_2, isAlive_2);
                }
                // else if (id2 == me->id)
                // {
                //     updateMe(isAlive_2, me);

                // }
                // // Else Update the player position on Window.
                // else
                // {
                //     updatePlayerPos(players, index, x2, y2, movement_2, nrOfPoints_2, isAlive_2);
                // }
            }
        }
        else if (sscanf((char *)recieve->data, "coins_response %d %d %d %d %d", &coinX, &coinY, &isVisible, &points, &coinId) == 5)
        {
            coins[*numberOfCoins] = (Coins){{coinX, coinY, COINS_MAX_W, COINS_MAX_H}, isVisible, points, *numberOfCoins};
            (*numberOfCoins)++;
            // printf("coins_data is recieved\n");
        }
        else if (sscanf((char *)recieve->data, "coins_data %d %d %d %d %d", &coinX, &coinY, &isVisible, &points, &coinId) == 5)
        {
            coins[coinId].isVisible = isVisible;
            // printf("coins_data has been updated\n");
        }
        // In case the recieved packet is not from our server.
        else
        {
            printf("Unknown message received: %s\n", (char *)packet->data);
        }
    }
}