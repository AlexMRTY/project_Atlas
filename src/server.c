#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_net.h"
#include "collisionDetection.h"
#include "const.h"
#include "player.h"
#include "world.h"

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDLNet_Init();

    UDPsocket server_socket = SDLNet_UDP_Open(SERVER_PORT);
    if (server_socket == NULL) {
        printf("Failed to open server socket: %s\n", SDLNet_GetError());
        return 1;
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    UDPpacket *recieve = SDLNet_AllocPacket(512);
    Player players[MAX_PLAYERS];
    int number_of_players = 0;

    while (1) {  // make it terminate on a exit command of sorts or SDL event.
        // Receive player updates and join requests
        while (SDLNet_UDP_Recv(server_socket, recieve)) {
            // printf("UDP Packet incoming\n");
            // printf("\tData:    %s\n", (char *)recieve->data);
            // printf("\tAddress: %x %x\n", recieve->address.host, recieve->address.port);

            int dx, dy, player_id;
            sscanf((char *)recieve->data, "%d %d %d", &dx, &dy, &player_id);

            if (strcmp((char *)recieve->data, "join_request") == 0 && number_of_players < MAX_PLAYERS) {
                Player player = {number_of_players, {32, 32, PLAYER_WIDTH, PLAYER_HIGHT}, recieve->address};
                players[number_of_players] = player;
                number_of_players++;
                // printf("Player joined with ID %d\n", player.id);

                packet->address.host = player.address.host;
                packet->address.port = player.address.port;
                sprintf((char *)packet->data, "join_accept %d %d %d", player.rect.x, player.rect.y, player.id);
                packet->len = strlen((char *)packet->data) + 1;
                SDLNet_UDP_Send(server_socket, -1, packet);

                memset(packet->data, 0, sizeof(packet->data));
            }

            for (int i = 0; i < number_of_players; i++) {
                if (players[i].id == player_id) {
                    // Update existing player's position
                    // collison detection will be moved to server side or be run on both.
                    if (collisionDetection(dx, dy)) {
                        printf("**************************Krock!\n");
                    } else {
                        printf("Ingen krock!\n");
                        players[i].rect.x = dx;
                        players[i].rect.y = dy;
                    }

                    // players[i].rect.x = dx;
                    // players[i].rect.y = dy;
                    //  printf("Update\n");
                    break;
                }
            }

            // Send player updates to all clients
            for (int i = 0; i < number_of_players; i++) {
                UDPpacket *update_packet = SDLNet_AllocPacket(512);
                update_packet->address.host = players[i].address.host;  // set host to the client address
                update_packet->address.port = players[i].address.port;  // set port to the client address

                for (int j = 0; j < number_of_players; j++) {
                    if (i != j) {
                        // sprintf((char *)update_packet->data, "player_data %d %d %d", players[j].rect.x, players[j].rect.y, players[j].id);
                        update_packet->len = strlen((char *)update_packet->data) + 1;

                        SDLNet_UDP_Send(server_socket, -1, update_packet);
                        // printf("Sending data to player %d\n", players[i].id);
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
