#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "SDL2/SDL_net.h"
#include "player.h"
#include "world.h"

#define SPEED 200 // 100
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024
#define PLAYER_MOVE_SPEED 32
#define PLAYER_HIGHT 32
#define PLAYER_WIDTH 32

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define MAX_PLAYERS 4

void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]);
void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);

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

    // Character
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

    const int NUM_SUBTEXTURES = 4;
    const int TEXTURE_WIDTH = 128;
    const int TEXTURE_HEIGHT = 128;
    const int SUBTEXTURE_WIDTH = 32;
    const int SUBTEXTURE_HEIGHT = 32;

    SDL_Rect subtextures[NUM_SUBTEXTURES];

    const int SUBTEXTURE_X_OFFSETS[NUM_SUBTEXTURES] = {0, SUBTEXTURE_WIDTH, 2 * SUBTEXTURE_WIDTH, 3 * SUBTEXTURE_WIDTH};
    const int SUBTEXTURE_Y_OFFSETS[NUM_SUBTEXTURES] = {0, SUBTEXTURE_HEIGHT, 2 * SUBTEXTURE_HEIGHT, 3 * SUBTEXTURE_HEIGHT};

    for (int i = 0; i < NUM_SUBTEXTURES; i++)
    {
        SDL_Rect subtextureRect = {SUBTEXTURE_X_OFFSETS[i], SUBTEXTURE_Y_OFFSETS[i], SUBTEXTURE_WIDTH, SUBTEXTURE_HEIGHT};
        subtextures[i] = subtextureRect;
    }

    // Background
    SDL_Texture *tTiles = NULL;
    SDL_Rect gTiles[8];

    loadTiles(pRenderer, &tTiles, gTiles);

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
    UDPpacket *recieve = SDLNet_AllocPacket(512);
    packet->address.host = server_address.host;
    packet->address.port = server_address.port;

    int number_of_player = 0;
    Player players[MAX_PLAYERS];
    Player me;
    bool joinedServer = false;
    bool quit = false;

    // Send request
    strcpy((char *)packet->data, "join_request");
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(client_socket, -1, packet);

    printf("Request Send\n");

    while (!quit)
    {
        // Receive updates from other players
        while (SDLNet_UDP_Recv(client_socket, recieve))
        {
            printf("UDP Packet from server\n");
            printf("\tData:    %s\n", (char *)recieve->data);
            printf("\tAddress: %x %x\n", recieve->address.host, recieve->address.port);

            int x, y, id;
            int x2, y2, id2;
            if (!joinedServer && sscanf((char *)recieve->data, "join_accept %d %d %d", &x, &y, &id) == 3 && number_of_player < MAX_PLAYERS)
            {
                printf("Joined server!\n");
                Player player = {id, {x, y, PLAYER_WIDTH, PLAYER_HIGHT}};
                players[number_of_player] = player;
                me = player;
                number_of_player++;
                joinedServer = true;
            }
            else if (sscanf((char *)recieve->data, "player_data %d %d %d", &x2, &y2, &id2) == 3)
            {
                printf("data from other players\n");
                int index = -1;

                if (number_of_player < MAX_PLAYERS)
                {
                    bool found = false;
                    for (int i = 0; i < number_of_player; i++)
                    {
                        if (players[i].id == id2)
                        {
                            found = true;
                            index = i;
                            break;
                        }
                    }
                    if (!found)
                    {
                        Player player = {id2, {x2, y2, PLAYER_WIDTH, PLAYER_HIGHT}};
                        players[number_of_player] = player;
                        number_of_player++;
                        printf("Added player with ID %d\n", id2);
                    }
                    else
                    {
                        players[index].rect.x = x2;
                        players[index].rect.y = y2;
                        printf("Player Already exist\n");
                    }
                }
            }
            else
            {
                printf("Unknown message received: %s\n", (char *)packet->data);
            }
        }

        // Get player input
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (me.rect.y - PLAYER_MOVE_SPEED >= 0)
                    {
                        me.rect.y -= PLAYER_MOVE_SPEED;
                    }
                    else
                    {
                        me.rect.y = 0;
                    }
                    me.movement = 1;
                    break;
                case SDLK_DOWN:
                    if (me.rect.y + PLAYER_MOVE_SPEED <= WINDOW_HEIGHT - PLAYER_HIGHT)
                    {
                        me.rect.y += PLAYER_MOVE_SPEED;
                    }
                    else
                    {
                        me.rect.y = WINDOW_HEIGHT - PLAYER_HIGHT;
                    }
                    me.movement = 2;
                    break;
                case SDLK_LEFT:
                    if (me.rect.x - PLAYER_MOVE_SPEED >= 0)
                    {
                        me.rect.x -= PLAYER_MOVE_SPEED;
                    }
                    else
                    {
                        me.rect.x = 0;
                    }
                    me.movement = 3;
                    break;
                case SDLK_RIGHT:
                    if (me.rect.x + PLAYER_MOVE_SPEED <= WINDOW_WIDTH - PLAYER_WIDTH)
                    {
                        me.rect.x += PLAYER_MOVE_SPEED;
                    }
                    else
                    {
                        me.rect.x = WINDOW_WIDTH - PLAYER_WIDTH;
                    }
                    me.movement = 4;
                    break;
                }
            }
        }
        if (joinedServer)
        {
            // Send player position update to server
            sprintf((char *)packet->data, "%d %d %d", me.rect.x, me.rect.y, me.id);
            packet->len = strlen((char *)packet->data) + 1;
            SDLNet_UDP_Send(client_socket, -1, packet);

            // printf("Sending data\n");        // Commented out during testing
            // printf("MY ID: %d\n", me.id);    // Commented out during testing

            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
            SDL_RenderClear(pRenderer);
            // Render background
            renderMap(pRenderer, tTiles, gTiles);

            SDL_Rect currentSubtexture = subtextures[0]; // default subtexture
            // Render all players
            for (int i = 0; i < number_of_player; i++)
            {
                SDL_Rect rect = players[i].rect;
                SDL_RenderCopy(pRenderer, pTexture, NULL, &rect);
                // printf("Draw Players\n"); // Commented out during testing
            }

            if (me.movement == 1)
            {
                currentSubtexture = subtextures[0];
            }
            else if (me.movement == 3)
            {
                currentSubtexture = subtextures[3];
            }
            else if (me.movement == 2)
            {
                currentSubtexture = subtextures[2];
            }
            else if (me.movement == 4)
            {
                currentSubtexture = subtextures[1];
            }
            SDL_RenderCopy(pRenderer, pTexture, &currentSubtexture, &me.rect);
            SDL_RenderPresent(pRenderer);
        }
    }
}

SDL_DestroyTexture(pTexture);
SDL_DestroyRenderer(pRenderer);
SDL_DestroyWindow(pWindow);

SDLNet_FreePacket(packet);
SDLNet_UDP_Close(client_socket);
SDLNet_Quit();
SDL_Quit();

return 0;
}

void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[])
{
    SDL_Surface *gTilesSurface = IMG_Load("resources/tilemap.png");
    if (!gTilesSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(gRenderer);
        // SDL_DestroyWindow(Window);
        SDL_Quit();
    }
    *mTiles = SDL_CreateTextureFromSurface(gRenderer, gTilesSurface);
    for (int i = 0; i < 8; i++)
    {
        gTiles[i].x = i * getTileWidth();
        gTiles[i].y = 0;
        gTiles[i].w = getTileWidth();
        gTiles[i].h = getTileHeight();
    }
}

void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[])
{
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();

    for (int i = 0; i < getTileColumns(); i++)
    {
        for (int j = 0; j < getTileRows(); j++)
        {
            position.y = i * getTileHeight();
            position.x = j * getTileWidth();
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
}
