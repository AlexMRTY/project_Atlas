
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "SDL2/SDL_net.h"
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>

#include "headers/globalConst.h"
#include "headers/world.h"
#include "headers/events.h"
#include "headers/render.h"
#include "headers/client.h"
#include "headers/coins.h"
#include "headers/player.h"
#include "headers/pause.h"
#include "headers/endgame.h"
#include "headers/lobby.h"
#include "headers/start.h"
#include "headers/scoreList.h"
#include "headers/leaderboards.h"

void playGame(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numCoins, SDL_Renderer *pRenderer, TTF_Font *font, bool *quit, Mix_Chunk *music, Mix_Chunk *coinsSound, Mix_Chunk *deathSound, int *update, int *escapePressed, int *gameState, SDL_Texture *tTiles, SDL_Rect gTiles[], SDL_Texture *tCoins, SDL_Rect gCoins[], int frame, SDL_Texture *client_textures[], SDL_Rect subtextures[], SDL_Texture *ppTexture);
void startMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font, int *gameState, bool *joinedServer, UDPpacket *packet, UDPsocket *client_socket);
void lobby(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numCoins, SDL_Renderer *pRenderer, TTF_Font *font, bool *quit, Mix_Chunk *music, Mix_Chunk *coinsSound, Mix_Chunk *deathSound, int *update, int *escapePressed, int *gameState);

int main(int argv, char **args)
{
	///////////////////////////////////////////////////////////////////
	///**********************INITIALIZATIONS***********************///
	/////////////////////////////////////////////////////////////////

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
    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
    }


	///**********************FONT**********************************///
    // TTF(font) init
    if(TTF_Init() == 0){ // new code
        printf("Success to initialize the SDL_ttf library\n");
    }

	TTF_Font* font = TTF_OpenFont("resources/ka1.ttf", 40);
	if (font == NULL) {
    	printf("TTF_OpenFont failed: %s\n", TTF_GetError());
	}

	///**********************AUDIO*********************************///
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

    Mix_Music *gameMusic = Mix_LoadMUS("resources/background.mp3");
    if (gameMusic == NULL)
    {
        printf("Failed to load music! SDL2_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Chunk *deathSound = Mix_LoadWAV("resources/death.mp3");
    if (gameMusic == NULL)
    {
        printf("Failed to load music! SDL2_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Music *gameOver = Mix_LoadMUS("resources/gameOver.wav");
    if (gameMusic == NULL)
    {
        printf("Failed to load music! SDL2_mixer Error: %s\n", Mix_GetError());
    }

	///**********************NETWORK******************************///
	if (SDLNet_Init() < 0) 
	{
		printf("Failed to initialize SDL_Net: %s\n", SDLNet_GetError());
        return 1;
	}

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


	///**********************GRAPHICS****************************///

	
    ///******VARIABLES*******///
	const char *pngNames[4] = {"resources/healer_f.png",
                               "resources/mage_f.png",
                               "resources/mage_m.png",
                               "resources/ninja_f.png"};

    const int SUBTEXTURE_X_OFFSETS[NUM_SUBTEXTURES] = {0, SUBTEXTURE_WIDTH, 2 * SUBTEXTURE_WIDTH, 3 * SUBTEXTURE_WIDTH};
    const int SUBTEXTURE_Y_OFFSETS[NUM_SUBTEXTURES] = {0, SUBTEXTURE_HEIGHT, 2 * SUBTEXTURE_HEIGHT, 3 * SUBTEXTURE_HEIGHT};

    Player players[MAX_PLAYERS];
    Player me;

	Coins coins[MAX_COINS];

    int gameState = 1;

	int numCoins = 0;
    int frame = 0;
    int update = -1;
    int number_of_player = 0;
    bool quit = false;

	// Menu flags
    int escapePressed = 0;
    
    long long int startingTick = SDL_GetTicks();
    long long int nrOfFPS = 0;

    // Timer variables
    int minutes, seconds;
    char timerText[20];

    bool joinedServer = false;


    ///******RECTS************************///
    SDL_Rect gTiles[8];
    SDL_Rect gCoins[8];

	SDL_Rect subtextures[NUM_SUBTEXTURES];
    for (int i = 0; i < NUM_SUBTEXTURES; i++)
    {
        SDL_Rect subtextureRect = {SUBTEXTURE_X_OFFSETS[i], SUBTEXTURE_Y_OFFSETS[i], SUBTEXTURE_WIDTH, SUBTEXTURE_HEIGHT};
        subtextures[i] = subtextureRect;
    }

	

    ///******SURFACES***********************///
    SDL_Surface *pSurface_one = IMG_Load(pngNames[0]);
    SDL_Surface *pSurface_two = IMG_Load(pngNames[1]);
    SDL_Surface *pSurface_three = IMG_Load(pngNames[2]);
    SDL_Surface *pSurface_four = IMG_Load(pngNames[3]);

	// PROPS
	SDL_Surface *gameOverPNG = IMG_Load("resources/pngegg.png");


    

	

    ///******TEXTURES**********************///

	// PLAYERS
    SDL_Texture *client_textures[4];
    client_textures[0] = SDL_CreateTextureFromSurface(pRenderer, pSurface_one);
    client_textures[1] = SDL_CreateTextureFromSurface(pRenderer, pSurface_two);
    client_textures[2] = SDL_CreateTextureFromSurface(pRenderer, pSurface_three);
    client_textures[3] = SDL_CreateTextureFromSurface(pRenderer, pSurface_four);

	// PROPS
    SDL_Texture *ppTexture = SDL_CreateTextureFromSurface(pRenderer, gameOverPNG);
    SDL_Texture *tCoins = NULL;

    // BACKGROUND
    SDL_Texture *tTiles = NULL;

    loadTiles(pRenderer, &tTiles, gTiles);;
	
	loadCoins(pRenderer, &tCoins, coins, &numCoins, gCoins);


    while (!quit)
    {
        switch (gameState)
        {
        case START_MENU:
            startMenu(pRenderer, &quit, font, &gameState, &joinedServer, packet, &client_socket);
            break;
        case LOBBY:
            me.isAlive = 1; // makes the game work on windows
            lobby(&client_socket, recieve, packet, players, &me, &number_of_player, &joinedServer, coins, &numCoins, pRenderer, font, &quit, music, coinsSound, deathSound, &update, &escapePressed, &gameState);
            break;
        case PLAY_GAME:
            playGame(&client_socket, recieve, packet, players, &me, &number_of_player, &joinedServer, coins, &numCoins, pRenderer, font, &quit, music, coinsSound, deathSound, &update, &escapePressed, &gameState, tTiles, gTiles, tCoins, gCoins, frame, client_textures, subtextures, ppTexture);
            break;
        case PAUSE_MENU:
            pauseMenu(pRenderer, &gameState, &quit, font);
            break;
        case LEADERBOARDS:
            // printf("leaderboards Active.\n");
            displayLeaderboard(pRenderer, &gameState, &quit, font, players);
            /* code */
            break;
        
        default:
            break;
        }
    }

    //Shutdown SDL
    Mix_HaltMusic();
    Mix_FreeMusic(gameMusic);
    Mix_CloseAudio();
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDLNet_FreePacket(packet);
    SDLNet_UDP_Close(client_socket);
    SDLNet_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}


void playGame(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numCoins, SDL_Renderer *pRenderer, TTF_Font *font, bool *quit, Mix_Chunk *music, Mix_Chunk *coinsSound, Mix_Chunk *deathSound, int *update, int *escapePressed, int *gameState, SDL_Texture *tTiles, SDL_Rect gTiles[], SDL_Texture *tCoins, SDL_Rect gCoins[], int frame, SDL_Texture *client_textures[], SDL_Rect subtextures[], SDL_Texture *ppTexture)
{
    HandleUDPRecv(client_socket, recieve, packet, players, me, number_of_player, joinedServer, coins, numCoins);

    // if (!joinedServer) return;

    // Handles quit and movement events
    handleEvents(&me->rect, &me->movement, quit, music, players, me->id, number_of_player, &me->numberOfPoints, coins, coinsSound, update, deathSound, escapePressed, me->isAlive, gameState, packet, client_socket);

    // Transmit cordinates data to server
    transmitData(me, packet, client_socket);

    // Transmit coins data to server
    transmitCoins(coins, *numCoins, packet, client_socket, *update);

    // if (me->isHunter) 
    // {
    //     transmittDiedPlayer(packet, client_socket, players, *number_of_player);
    // }

    // Clear Window
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pRenderer);

    // Render background
    renderMap(pRenderer, tTiles, gTiles);

    // Render all coins
    renderCoins(pRenderer, &tCoins, coins, *numCoins, gCoins, frame);

    // Render all players
    renderPlayers(pRenderer, client_textures, subtextures, NUM_SUBTEXTURES, players, *number_of_player, *me, ppTexture);

    // TESTING GAMEOVER/VICTORY
    if (!me->isAlive) {
        endGame(pRenderer, font, 0);
    }
    
    // Render frame
    SDL_RenderPresent(pRenderer); 
}

void startMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font, int *gameState, bool *joinedServer, UDPpacket *packet, UDPsocket *client_socket)
{
    displayStartMenu(pRenderer, quit, font, gameState);
    
    if (*gameState == 2) // lobby
    {
        strcpy((char *)packet->data, "join_request");
        packet->len = strlen((char *)packet->data) + 1;
        SDLNet_UDP_Send((*client_socket), -1, packet);

        strcpy((char *)packet->data, "coins_request");
        packet->len = strlen((char *)packet->data) + 1;
        SDLNet_UDP_Send((*client_socket), -1, packet);
        (*joinedServer) = false;

        printf("Request Send\n");
    }
}

void lobby(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numCoins, SDL_Renderer *pRenderer, TTF_Font *font, bool *quit, Mix_Chunk *music, Mix_Chunk *coinsSound, Mix_Chunk *deathSound, int *update, int *escapePressed, int *gameState)
{
    HandleUDPRecv(client_socket, recieve, packet, players, me, number_of_player, joinedServer, coins, numCoins);

    handleEvents(&me->rect, &me->movement, quit, music, players, me->id, number_of_player, &me->numberOfPoints, coins, coinsSound, update, deathSound, escapePressed, me->isAlive, gameState, packet, client_socket);

    transmitData(me, packet, client_socket);

    waitingForPlayers(pRenderer, font, *number_of_player);

    if ((*number_of_player) == 4) 
    {
        (*gameState) = 3; 
    }

}