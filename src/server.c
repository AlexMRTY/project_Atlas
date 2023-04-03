#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"

#include "player.h"

#define SERVER_PORT 2000


void move_player(Player* player, int dx, int dy) {
    player->rect.x += dx;
    player->rect.y += dy;
}

void update_players(Player* players, int num_players, UDPsocket socket, UDPpacket* packet) {
    for (int i = 0; i < num_players; i++) {
        if (i == packet->data[2] - 1) continue; // Don't send to the player who sent the packet

        packet->address.host = SDL_SwapBE32(players[i].ip_address);
        packet->address.port = SDL_SwapBE16(players[i].port);
        sprintf((char*) packet->data, "%d %d %d", players[packet->data[2] - 1].rect.x, players[packet->data[2] - 1].rect.y, packet->data[2]);
        packet->len = strlen((char*) packet->data) + 1;

        SDLNet_UDP_Send(socket, -1, packet);
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDLNet_Init();

    UDPsocket server_socket = SDLNet_UDP_Open(SERVER_PORT);
    if (server_socket == NULL) {
        printf("Failed to open socket: %s\n", SDLNet_GetError());
        return 1;
    }

    UDPpacket* packet = SDLNet_AllocPacket(512);

    Player players[2] = {
        {1, {0, 0, 50, 50}},
        {2, {250, 250, 50, 50}}
    };
    int num_players = 0;

    while (1) {
        if (SDLNet_UDP_Recv(server_socket, packet)) {
            printf("UDP Packet incoming\n");
            printf("\tData:    %s\n", (char*) packet->data);
            printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);

            if (num_players < 2) {
                // Add new player
                players[num_players].ip_address = SDL_SwapBE32(packet->address.host);
                players[num_players].port = SDL_SwapBE16(packet->address.port);
                num_players++;
            } else {
                // Update player position
                int player_id = packet->data[2] - 1;
                int dx, dy;
                sscanf((char*) packet->data, "%d %d %d", &dx, &dy, &player_id);

                move_player(&players[player_id - 1], dx, dy);
                update_players(players, num_players, server_socket, packet);
            }
        }
    }

    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(server_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
