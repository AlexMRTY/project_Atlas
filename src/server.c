#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"

#include "player.h"

#define SERVER_PORT 2000
#define MAX_PLAYERS 4

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

    Player players[MAX_PLAYERS] = {0};
    int number_of_players = 0;

    while (1)
    {
        // Receive player updates
        while (SDLNet_UDP_Recv(server_socket, packet))
        {
            printf("UDP Packet incoming\n");
            printf("\tData:    %s\n", (char *)packet->data);
            printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);

            int dx, dy, player_id;
            sscanf((char *)packet->data, "%d %d %d", &dx, &dy, &player_id);

            bool player_found = false;
            for (int i = 0; i < number_of_players; i++)
            {
                if (players[i].id == player_id)
                {
                    // Update existing player's position
                    players[i].rect.x = dx;
                    players[i].rect.y = dy;
                    player_found = true;
                    break;
                }
            }

            if (!player_found && number_of_players < MAX_PLAYERS)
            {
                // Add new player
                Player new_player = {player_id, {dx, dy, 50, 50}, packet->address};
                players[number_of_players] = new_player;
                number_of_players++;
                printf("The code has Successfully added a new player to the server ");
            }
        }

        // Send player updates to all clients
        for (int i = 0; i < number_of_players; i++)
        {
            Player *player = &players[i];

            sprintf((char *)packet->data, "%d %d %d", player->rect.x, player->rect.y, player->id);
            packet->len = strlen((char *)packet->data) + 1;

            for (int j = 0; j < number_of_players; j++)
            {
                if (i != j)
                {
                    packet->address.host = players[j].address.host;
                    packet->address.port = players[j].address.port;
                    SDLNet_UDP_Send(server_socket, -1, packet);
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
