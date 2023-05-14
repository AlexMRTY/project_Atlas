#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/globalConst.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"

#include "headers/player.h"
#include "headers/world.h"
#include "headers/collisionDetection.h"
#include "headers/coins.h"

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDLNet_Init();

    UDPsocket server_socket = SDLNet_UDP_Open(SERVER_PORT);
    if (server_socket == NULL)
    {
        printf("Failed to open server socket: %s\n", SDLNet_GetError());
        return 1;
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    UDPpacket *recieve = SDLNet_AllocPacket(512);
    Player players[MAX_PLAYERS];
    Coins coins[MAX_COINS];
    int numberOfCoins = 0;
    int number_of_players = 0;

    long long int startingTick = SDL_GetTicks();
    long long int nrOfFPS = 0;


    // Generate a random number between 0 and 3
    srand(time(NULL));
    int choosenHunter = rand() % 4;

    initializeCoins(coins, &numberOfCoins);

    printf("number of coins: %d\n", numberOfCoins);

    while (1)
    {
        // Receive player updates and join requests
        while (SDLNet_UDP_Recv(server_socket, recieve))
        {
            long long int tick = SDL_GetTicks();
            nrOfFPS++;

            if (nrOfFPS % 30 == 0) // every 30fps
            {
                float avgFPS = (float)nrOfFPS / ((tick - startingTick) / 1000.f);
                // printf("avg fps: %.2f\n", avgFPS);
            }
            if (nrOfFPS % 150 == 0)
            {
                startingTick = tick; // resetting fps ctr;
                nrOfFPS = 0;
            }

            int dx, dy, player_id, movement, nrOfPoints, isAlive, coinX, coinY, isVisible, points, coinId, killId, killIsAlive;
            sscanf((char *)recieve->data, "%d %d %d %d %d %d", &dx, &dy, &player_id, &nrOfPoints, &movement, &isAlive);

            if (strcmp((char *)recieve->data, "join_request") == 0 && number_of_players < MAX_PLAYERS)
            {
                Player player = initializePlayer(&number_of_players, players, recieve, choosenHunter);
                printf("Player joined with ID %d\n", player.id);
                printf("Number of players> %d\n", number_of_players);
                printf("Hunter is > %d\n", choosenHunter);


                packet->address.host = player.address.host;
                packet->address.port = player.address.port;
                sprintf((char *)packet->data, "join_accept %d %d %d %d %d %d %d", player.rect.x, player.rect.y, player.id, player.numberOfPoints, player.movement, player.isAlive, player.isHunter);
                packet->len = strlen((char *)packet->data) + 1;
                SDLNet_UDP_Send(server_socket, -1, packet);
            }

            else if (sscanf((char *)recieve->data, "coins_update %d %d %d %d %d", &coinX, &coinY, &isVisible, &points, &coinId) == 5)
            {
                for (int j = 0; j < numberOfCoins; j++)
                {
                    if (coins[j].id == coinId)
                    {
                        updateCoins(coins, coinId, isVisible);
                        // printf("Updating Coins!\n");
                    }
                }
                for (int i = 0; i < number_of_players; i++)
                {
                    UDPpacket *update_packet = SDLNet_AllocPacket(512);
                    update_packet->address.host = players[i].address.host; // set host to the client address
                    update_packet->address.port = players[i].address.port; // set port to the client address

                    sprintf((char *)update_packet->data, "coins_data %d %d %d %d %d", coins[coinId].coin.x, coins[coinId].coin.y, coins[coinId].isVisible, coins[coinId].points, coins[coinId].id);
                    update_packet->len = strlen((char *)update_packet->data) + 1;
                    SDLNet_UDP_Send(server_socket, -1, update_packet);
                    // printf("Sending Coins data to player %d\n", players[i].id);

                    SDLNet_FreePacket(update_packet);
                }
            }
            else if (strcmp((char *)recieve->data, "coins_request") == 0)
            {
                UDPpacket *update_packet = SDLNet_AllocPacket(512);
                update_packet->address.host = recieve->address.host; // set host to the client address
                update_packet->address.port = recieve->address.port; // set port to the client address
                for (int j = 0; j < numberOfCoins; j++)
                {
                    sprintf((char *)update_packet->data, "coins_response %d %d %d %d %d", coins[j].coin.x, coins[j].coin.y, coins[j].isVisible, coins[j].points, coins[j].id);
                    update_packet->len = strlen((char *)update_packet->data) + 1;

                    SDLNet_UDP_Send(server_socket, -1, update_packet);
                    // printf("Sending Coins data to player \n");
                }

                SDLNet_FreePacket(update_packet);
            }
            else if (sscanf((char *)recieve->data, "new kill %d", &killId) == 1)
            {
                updateIsAlive(killId, number_of_players, players);
                printf("new kill...\n");
            }

            for (int i = 0; i < number_of_players; i++)
            {
                if (players[i].id == player_id)
                {
                    updatePlayerPos(players, i, dx, dy, movement, nrOfPoints, isAlive);
                    break;
                }
            }

            // Send player updates to all clients
            if (nrOfFPS % 3 == 0)
            { // every 3rd frame/tick
                for (int i = 0; i < number_of_players; i++)
                {
                    UDPpacket *update_packet = SDLNet_AllocPacket(512);
                    update_packet->address.host = players[i].address.host; // set host to the client address
                    update_packet->address.port = players[i].address.port; // set port to the client address

                    for (int j = 0; j < number_of_players; j++)
                    {
                        //     if (i != j)
                        //    {
                        sprintf((char *)update_packet->data, "player_data %d %d %d %d %d %d %d", players[j].rect.x, players[j].rect.y, players[j].id, players[j].numberOfPoints, players[j].movement, players[j].isAlive, players[j].isHunter);
                        update_packet->len = strlen((char *)update_packet->data) + 1;

                        SDLNet_UDP_Send(server_socket, -1, update_packet);
                        // printf("Sending data to player %d\n", players[i].id);
                        //   }
                    }
                    SDLNet_FreePacket(update_packet);
                }
            }
        }
    }

    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(server_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}