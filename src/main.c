#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "SDL2/SDL_net.h"
#include "globalConst.h"
#include "player.h"
#include "world.h"
#include "events.h"
// #include "collisionDetection.h"

// #define SPEED 200 // 100
// #define WINDOW_WIDTH 1024
// #define WINDOW_HEIGHT 1024
// #define PLAYER_MOVE_SPEED 32
// #define PLAYER_HIGHT 32
// #define PLAYER_WIDTH 32

// #define NUM_SUBTEXTURES 4
// #define TEXTURE_WIDTH 128
// #define TEXTURE_HEIGHT 128
// #define SUBTEXTURE_WIDTH 32
// #define SUBTEXTURE_HEIGHT 32

// #define SERVER_IP "127.0.0.1"
// #define SERVER_PORT 12345
// #define MAX_PLAYERS 4

void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]);
void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);
void renderPlayers(SDL_Renderer *pRenderer, SDL_Texture **pTextures, SDL_Rect *subtextures, int num_subtextures, Player *players, int num_players, Player me);

int main(int argv, char **args)
{

    const char *pngNames[4] = {"resources/healer_f.png",
                               "resources/mage_f.png",
                               "resources/mage_m.png",
                               "resources/ninja_f.png"};

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

    // const int NUM_SUBTEXTURES = 4;
    // const int TEXTURE_WIDTH = 128;
    // const int TEXTURE_HEIGHT = 128;
    // const int SUBTEXTURE_WIDTH = 32;
    // const int SUBTEXTURE_HEIGHT = 32;

    SDL_Rect subtextures[NUM_SUBTEXTURES];

    const int SUBTEXTURE_X_OFFSETS[NUM_SUBTEXTURES] = {0, SUBTEXTURE_WIDTH, 2 * SUBTEXTURE_WIDTH, 3 * SUBTEXTURE_WIDTH};
    const int SUBTEXTURE_Y_OFFSETS[NUM_SUBTEXTURES] = {0, SUBTEXTURE_HEIGHT, 2 * SUBTEXTURE_HEIGHT, 3 * SUBTEXTURE_HEIGHT};

    for (int i = 0; i < NUM_SUBTEXTURES; i++)
    {
        SDL_Rect subtextureRect = {SUBTEXTURE_X_OFFSETS[i], SUBTEXTURE_Y_OFFSETS[i], SUBTEXTURE_WIDTH, SUBTEXTURE_HEIGHT};
        subtextures[i] = subtextureRect;
    }

    // players Textures and Imgaes
    SDL_Surface *pSurface_one = IMG_Load(pngNames[0]);
    SDL_Surface *pSurface_two = IMG_Load(pngNames[1]);
    SDL_Surface *pSurface_three = IMG_Load(pngNames[2]);
    SDL_Surface *pSurface_four = IMG_Load(pngNames[3]);

    SDL_Texture *client_textures[4];
    client_textures[0] = SDL_CreateTextureFromSurface(pRenderer, pSurface_one);
    client_textures[1] = SDL_CreateTextureFromSurface(pRenderer, pSurface_two);
    client_textures[2] = SDL_CreateTextureFromSurface(pRenderer, pSurface_three);
    client_textures[3] = SDL_CreateTextureFromSurface(pRenderer, pSurface_four);

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

            int x, y, id, nrOfpoints, movement;
            int x2, y2, id2, nrOfpoints_2, movement_2;
            if (!joinedServer && sscanf((char *)recieve->data, "join_accept %d %d %d %d %d", &x, &y, &id, &nrOfpoints, &movement) == 5 && number_of_player <= MAX_PLAYERS)
            {
                printf("Joined server!\n");
                Player player = {id, {x, y, PLAYER_WIDTH, PLAYER_HIGHT}, nrOfpoints, movement};
                players[number_of_player] = player;
                me = player;
                number_of_player++;
                joinedServer = true;
            }
            else if (sscanf((char *)recieve->data, "player_data %d %d %d %d %d", &x2, &y2, &id2, &nrOfpoints_2, &movement_2) == 5)
            {
                printf("data from other players\n");
                int index = -1;

                if (number_of_player <= MAX_PLAYERS)
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
                        Player player = {id2, {x2, y2, PLAYER_WIDTH, PLAYER_HIGHT}, nrOfpoints_2, movement_2};
                        players[number_of_player] = player;
                        number_of_player++;
                        printf("Added player with ID %d\n", id2);
                    }
                    else
                    {
                        players[index].rect.x = x2;
                        players[index].rect.y = y2;
                        players[index].movement = movement_2;
                        players[index].numberOfPoints = nrOfpoints_2;
                        printf("Player Already exist\n");
                    }
                }
            }
            else
            {
                printf("Unknown message received: %s\n", (char *)packet->data);
            }
        }

        handleEvents(&me.rect, &me.movement, &quit);    // Handles quit and movement events
     
        if (joinedServer)
        {
            // Send player position update to server
            sprintf((char *)packet->data, "%d %d %d %d %d", me.rect.x, me.rect.y, me.id, me.numberOfPoints, me.movement);
            packet->len = strlen((char *)packet->data) + 1;
            SDLNet_UDP_Send(client_socket, -1, packet);

            // printf("Sending data\n");        // Commented out during testing
            // printf("MY ID: %d\n", me.id);    // Commented out during testing

            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
            SDL_RenderClear(pRenderer);
            // Render background
            renderMap(pRenderer, tTiles, gTiles);
            renderPlayers(pRenderer, client_textures, subtextures, NUM_SUBTEXTURES, players, number_of_player, me);
        }
    }

    //   SDL_DestroyTexture(pTexture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(client_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}

void renderPlayers(SDL_Renderer *pRenderer, SDL_Texture **pTexture, SDL_Rect *subtextures, int num_subtextures, Player *players, int num_players, Player me)
{
    // Change current subtexture based on player movement
    SDL_Rect currentSubtexture;
    if (me.movement == 1)
    {
        currentSubtexture = subtextures[0];
    }
    else if (me.movement == 3)
    {
        currentSubtexture = subtextures[1];
        SDL_Texture *playerTexture = pTexture[me.id];
        SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &me.rect, 0, NULL, flip);
    }
    else if (me.movement == 2)
    {
        currentSubtexture = subtextures[2];
    }
    else if (me.movement == 4)
    {
        currentSubtexture = subtextures[1];
    }

    // Render current player with correct subtexture
    SDL_Texture *playerTexture = pTexture[me.id];
    SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &me.rect, 0, NULL, SDL_FLIP_NONE);

    // Render all other players
    for (int i = 0; i < num_players; i++)
    {
        if (players[i].id != me.id)
        {
            if (players[i].movement == 1)
            {
                currentSubtexture = subtextures[0];
            }
            else if (players[i].movement == 3)
            {
                currentSubtexture = subtextures[1];
                SDL_Texture *playerTexture = pTexture[players[i].id];
                SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
                SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &players[i].rect, 0, NULL, flip);
            }
            else if (players[i].movement == 2)
            {
                currentSubtexture = subtextures[2];
            }
            else if (players[i].movement == 4)
            {
                currentSubtexture = subtextures[1];
            }

            // Render current player with correct subtexture
            SDL_Texture *playerTexture = pTexture[players[i].id];
            SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &players[i].rect, 0, NULL, SDL_FLIP_NONE);
        }
    }

    SDL_RenderPresent(pRenderer);
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