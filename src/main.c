
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

#include "SDL2/SDL_net.h"
#include "globalConst.h"
#include "world.h"
#include "events.h"
#include "render.h"
#include "client.h"

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
    
    bool quit = false;

    // Send request
    strcpy((char *)packet->data, "join_request");
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(client_socket, -1, packet);

    bool joinedServer = false;

    printf("Request Send\n");

    while (!quit)
    {
        HandleUDPRecv(&client_socket, recieve, packet, players, &me, &number_of_player, &joinedServer);

        // Receive updates from other players
        // while (SDLNet_UDP_Recv(client_socket, recieve))
        // {
        //     // Prints the recieved player data to terminal.
        //     printPlayerData(recieve); 

        //     int x, y, id, nrOfPoints, movement;
        //     int x2, y2, id2, nrOfpoints_2, movement_2;
            
        //     if (!*(&joinedServer) && 
        //     joinAccept(recieve, &x, &y, &id, &nrOfPoints, &movement) == 5 && 
        //     *(&number_of_player) <= MAX_PLAYERS)
        //     {
        //         printf("Joined server!\n");
        //         *(&me) = addPlayer(&id, &x, &y, &nrOfPoints, &movement, players, &number_of_player);
        //         *(&joinedServer) = true;

        //     }
        //     else if (recievePlayerData(recieve, &x2, &y2, &id2, &nrOfpoints_2, &movement_2) == 5)
        //     {
        //         printf("data from other players\n");
        //         int index = -1;

        //         if (number_of_player <= MAX_PLAYERS)
        //         {
        //             // Look if player already exists
        //             int found = playerExists(&number_of_player, players, &index, id2);

        //             // If player does not exist, add new player.
        //             if (!found)
        //             {
        //                 addPlayer(&id2, &x2, &y2, &nrOfpoints_2, &movement_2, players, &number_of_player);
        //                 printf("Added player with ID %d\n", id2);
        //             } else {
        //                 updatePlayerPos(players, index, x2, y2, movement_2, nrOfpoints_2);
                
        //             }
        //         }
        //     }
        //     else
        //     {
        //         printf("Unknown message received: %s\n", (char *)packet->data);
        //     }
        // }









        // Receive updates from other players
        // while (SDLNet_UDP_Recv(client_socket, recieve))
        // {
        //     printf("UDP Packet from server\n");
        //     printf("\tData:    %s\n", (char *)recieve->data);
        //     printf("\tAddress: %x %x\n", recieve->address.host, recieve->address.port); 

        //     int x, y, id, nrOfpoints, movement;
        //     int x2, y2, id2, nrOfpoints_2, movement_2;
        //     if (!joinedServer && sscanf((char *)recieve->data, "join_accept %d %d %d %d %d", &x, &y, &id, &nrOfpoints, &movement) == 5 && number_of_player <= MAX_PLAYERS)
        //     {
        //         printf("Joined server!\n");
        //         Player player = {id, {x, y, PLAYER_WIDTH, PLAYER_HIGHT}, nrOfpoints, movement};
        //         players[number_of_player] = player;
        //         me = player;
        //         number_of_player++;
        //         joinedServer = true;
        //     }
        //     else if (sscanf((char *)recieve->data, "player_data %d %d %d %d %d", &x2, &y2, &id2, &nrOfpoints_2, &movement_2) == 5)
        //     {
        //         printf("data from other players\n");
        //         int index = -1;

        //         if (number_of_player <= MAX_PLAYERS)
        //         {
        //             bool found = false;
        //             for (int i = 0; i < number_of_player; i++)
        //             {
        //                 if (players[i].id == id2)
        //                 {
        //                     found = true;
        //                     index = i;
        //                     break;
        //                 }
        //             }
        //             if (!found)
        //             {
        //                 Player player = {id2, {x2, y2, PLAYER_WIDTH, PLAYER_HIGHT}, nrOfpoints_2, movement_2};
        //                 players[number_of_player] = player;
        //                 number_of_player++;
        //                 printf("Added player with ID %d\n", id2);
        //             }
        //             else
        //             {
        //                 players[index].rect.x = x2;
        //                 players[index].rect.y = y2;
        //                 players[index].movement = movement_2;
        //                 players[index].numberOfPoints = nrOfpoints_2;
        //                 printf("Player Already exist\n");
        //             }
        //         }
        //     }
        //     else
        //     {
        //         printf("Unknown message received: %s\n", (char *)packet->data);
        //     }
        // }

        // Handles quit and movement events
        handleEvents(&me.rect, &me.movement, &quit);    
     
        if (joinedServer)
        {
            // Transmit my data to others
            transmitData(&me, packet, &client_socket);

            // Clear Window
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
            SDL_RenderClear(pRenderer);
            
            // Render background
            renderMap(pRenderer, tTiles, gTiles);

            // Render all players
            renderPlayers(pRenderer, client_textures, subtextures, NUM_SUBTEXTURES, players, number_of_player, me);
        }
    }

    //   Close SDL
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(client_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}