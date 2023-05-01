
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "SDL2/SDL_net.h"

#include <stdbool.h>
#include <stdio.h>

#include "globalConst.h"
#include "world.h"
#include "events.h"
#include "render.h"
#include "client.h"
#include "coins.h"
#include "player.h"

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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL2_mixer could not initialize! SDL2_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Chunk *music = Mix_LoadWAV("resources/sfx_step_rock_l.mp3");
    if (music == NULL)
    {
        printf("Failed to load music! SDL2_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Chunk *coinsSound = Mix_LoadWAV("resources/mixkit-money-bag-drop-1989.wav");
    if (music == NULL)
    {
        printf("Failed to load music! SDL2_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Music *gameMusic = Mix_LoadMUS("resources/Spazzmatica-Polka.mp3");
    if (gameMusic == NULL)
    {
        printf("Failed to load music! SDL2_mixer Error: %s\n", Mix_GetError());
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

    SDL_Texture *tCoins = NULL;
    int numCoins = 0;
    Coins coins[MAX_COINS];
    SDL_Rect gCoins[8];
    int frame = 0;
    int update = -1;

    loadTiles(pRenderer, &tTiles, gTiles);

    loadCoins(pRenderer, &tCoins, coins, &numCoins, gCoins);

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

    strcpy((char *)packet->data, "coins_request");
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(client_socket, -1, packet);
    bool joinedServer = false;

    printf("Request Send\n");
    Mix_PlayMusic(gameMusic, -1);

    long long int startingTick = SDL_GetTicks();
    long long int nrOfFPS = 0;

    while (!quit && !isGameOver(me))
    {
        // Handle UDP packet recieved from Server.
        HandleUDPRecv(&client_socket, recieve, packet, players, &me, &number_of_player, &joinedServer, coins, &numCoins);

        // Handles quit and movement events

        if (joinedServer)
        {
            long long int tick = SDL_GetTicks();
            nrOfFPS++;

            if (nrOfFPS % 30 == 0) // every 30fps
            {
                float avgFPS = (float)nrOfFPS / ((tick - startingTick) / 1000.f);
                printf("avg fps: %.2f\n", avgFPS);
            }
            if (nrOfFPS % 150 == 0)
            {
                startingTick = tick; // resetting fps ctr;
                nrOfFPS = 0;
            }
            handleEvents(&me.rect, &me.movement, &quit, music, players, me.id, &number_of_player, &me.numberOfPoints, coins, coinsSound, &update);

            // Transmit my data to others
            transmitData(&me, packet, &client_socket);

            transmitCoins(coins, numCoins, packet, &client_socket, update);

            if (isMonster(me.id))
                transmittDiedPlayer(packet, &client_socket, players, number_of_player);

            // Clear Window
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
            SDL_RenderClear(pRenderer);

            // Render background
            renderMap(pRenderer, tTiles, gTiles);

            // Render all coins
            renderCoins(pRenderer, &tCoins, coins, numCoins, gCoins, frame);

            // Render all players
            renderPlayers(pRenderer, client_textures, subtextures, NUM_SUBTEXTURES, players, number_of_player, me);

            frame++;
        }
    }
    //   Close SDL
    Mix_HaltMusic();
    Mix_FreeMusic(gameMusic);
    Mix_CloseAudio();
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(client_socket);
    SDLNet_Quit();
    SDL_Quit();

    return 0;
}