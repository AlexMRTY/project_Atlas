#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_net.h"

#include "player.h"

#define SPEED 200 // 100
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 2000

int main(int argv, char **args)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *pWindow = SDL_CreateWindow("Enkelt exempel 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!pWindow)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!pRenderer)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *pSurface = IMG_Load("resources/ship.png");
    if (!pSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    SDL_FreeSurface(pSurface);
    if (!pTexture)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

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

    int number_of_player = 0;
    Player player = {0, {100, 100, 50, 50}, packet->address}; // Create new player at (100, 100)

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
            printf("UDP Packet from another player\n");
            printf("\tData:    %s\n", (char *)packet->data);
            printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);

            int dx, dy, player_id;
            sscanf((char *)packet->data, "%d %d %d", &dx, &dy, &player_id);

            player.rect.x = dx;
            player.rect.y = dy;
        }

        // Clear screen
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        // Render players
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(pRenderer, &player.rect);

        SDL_RenderPresent(pRenderer);
    }

quit:
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(client_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}