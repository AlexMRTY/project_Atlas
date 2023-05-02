
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>
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

    // COUNT DOWN CODE, MUST BE OPTIMIZED
    // SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Countdown", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("resources/Roboto-Light.ttf", 48);
    printf("open font\n");
    int countdownDuration = 10; // in seconds
    int countdownValue = countdownDuration;
    SDL_Event event;
    int quit1 = 0;

    while (!quit1)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit1 = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color
        SDL_RenderClear(renderer);                      // Clear the renderer

        // Render the countdown text
        char countdownText[20];
        sprintf(countdownText, "Time Left: %d", countdownValue);
        SDL_Color textColor = {255, 255, 255}; // White color
        SDL_Surface *surface = TTF_RenderText_Solid(font, countdownText, textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        // Determine the position of the text
        SDL_Rect textRect;
        textRect.x = 400 - surface->w / 2; // Centered horizontally
        textRect.y = 300 - surface->h / 2; // Centered vertically

        SDL_RenderCopy(renderer, texture, NULL, &textRect); // Render the countdown text
        SDL_RenderPresent(renderer);                        // Update the screen

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        // Update the countdown
        countdownValue--;
        if (countdownValue < 0)
        {
            quit1 = 1; // Exit the loop when countdown reaches 0
        }

        SDL_Delay(1000); // Delay for 1 second
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
        // Handle UDP packet recieved from Server.
        HandleUDPRecv(&client_socket, recieve, packet, players, &me, &number_of_player, &joinedServer);

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