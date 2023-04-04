#include <stdio.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_net.h"

#include "player.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2000

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDLNet_Init();

    UDPsocket client_socket = SDLNet_UDP_Open(0);
    if (client_socket == NULL)
    {
        printf("Failed to open socket: %s\n", SDLNet_GetError());
        return 1;
    }

    IPaddress server_address;
    if (SDLNet_ResolveHost(&server_address, SERVER_IP, SERVER_PORT) == -1)
    {
        printf("Failed to resolve server address: %s\n", SDLNet_GetError());
        return 1;
    }

    UDPpacket *packet = SDLNet_AllocPacket(512);
    packet->address.host = server_address.host;
    packet->address.port = server_address.port;

    Player player = {0, {100, 100, 50, 50}}; // Create new player at (100, 100)

    while (1)
    {
        // Get player input
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                goto quit;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    player.rect.y -= 5;
                    break;
                case SDLK_DOWN:
                    player.rect.y += 5;
                    break;
                case SDLK_LEFT:
                    player.rect.x -= 5;
                    break;
                case SDLK_RIGHT:
                    player.rect.x += 5;
                    break;
                }
            }
        }

        // Send player position update to server
        sprintf((char *)packet->data, "%d %d %d", player.rect.x, player.rect.y, player.id);
        packet->len = strlen((char *)packet->data) + 1;
        SDLNet_UDP_Send(client_socket, -1, packet);

        // Receive updates from other players
        while (SDLNet_UDP_Recv(client_socket, packet))
        {
            printf("UDP Packet incoming\n");
            printf("\tData:    %s\n", (char *)packet->data);
            printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);

            int dx, dy, player_id;
            sscanf((char *)packet->data, "%d %d %d", &dx, &dy, &player_id);

            if (player_id != player.id)
            {
                // Update other player's position
                player.rect.x = dx;
                player.rect.y = dy;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render players
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &player.rect);

        SDL_RenderPresent(renderer);
    }

quit:
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(client_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
