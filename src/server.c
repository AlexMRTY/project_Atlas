#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "globalConst.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "player.h"
#include "world.h"

bool collisionDetection(int dx, int dy);
bool collisionWithMap(int dx, int dy);

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
    int number_of_players = 0;

    while (1)
    {
        // Receive player updates and join requests
        while (SDLNet_UDP_Recv(server_socket, recieve))
        {

            int dx, dy, player_id, movement, nrOfPoints;
            sscanf((char *)recieve->data, "%d %d %d %d %d", &dx, &dy, &player_id, &nrOfPoints, &movement);

            if (strcmp((char *)recieve->data, "join_request") == 0 && number_of_players < MAX_PLAYERS)
            {
                Player player = initializePlayer(&number_of_players, players, recieve);
                printf("Player joined with ID %d\n", player.id);

                packet->address.host = player.address.host;
                packet->address.port = player.address.port;
                sprintf((char *)packet->data, "join_accept %d %d %d %d %d", player.rect.x, player.rect.y, player.id, player.numberOfPoints, player.movement);
                packet->len = strlen((char *)packet->data) + 1;
                SDLNet_UDP_Send(server_socket, -1, packet);
            }

            for (int i = 0; i < number_of_players; i++)
            {
                if (players[i].id == player_id)
                {
                    updatePlayerPos(players, i, dx, dy, movement, nrOfPoints);
                    break;
                }
            }

            // Send player updates to all clients
            for (int i = 0; i < number_of_players; i++)
            {
                UDPpacket *update_packet = SDLNet_AllocPacket(512);
                update_packet->address.host = players[i].address.host; // set host to the client address
                update_packet->address.port = players[i].address.port; // set port to the client address

                for (int j = 0; j < number_of_players; j++)
                {
                    if (i != j)
                    {
                        sprintf((char *)update_packet->data, "player_data %d %d %d %d %d", players[j].rect.x, players[j].rect.y, players[j].id, players[j].numberOfPoints, players[j].movement);
                        update_packet->len = strlen((char *)update_packet->data) + 1;

                        SDLNet_UDP_Send(server_socket, -1, update_packet);
                        printf("Sending data to player %d\n", players[i].id);
                    }
                }

                SDLNet_FreePacket(update_packet);
            }
        }
    }

    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(server_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}