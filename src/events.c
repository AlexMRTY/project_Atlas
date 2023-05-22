#include "SDL2/SDL_mixer.h"
#include "headers/globalConst.h"
#include "headers/collisionDetection.h"
#include "headers/events.h"
#include "headers/coins.h"
#include "headers/player.h"


void handleEvents(SDL_Rect *rect, int *movement, bool *quit, Mix_Chunk *music, Player players[], int currentPlayer, int *nrOfPlayers, int *numberOfPoints, Coins coins[], Mix_Chunk *coinsSound, int *update, Mix_Chunk *deathSound, int *escapePressed, int isAlive, int *gameState, UDPpacket *packet, UDPsocket *client_socket)
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        // printf("pollevent: %d", SDL_PollEvent(&event));u
        *quit = handleQuit(&event);

        
        transformCharacter(&event, rect, movement, music, players, currentPlayer, nrOfPlayers, numberOfPoints, coins, coinsSound, update, deathSound, escapePressed, gameState, isAlive, packet, client_socket);
        
    }
}

bool handleQuit(SDL_Event *event)
{
    return (event->type == SDL_QUIT) ? true : false;
}

void transformCharacter(SDL_Event *event, SDL_Rect *rect, int *movement, Mix_Chunk *music, Player players[], int currentPlayer, int *nrOfPlayers, int *numberOfPoints, Coins coins[], Mix_Chunk *coinsSound, int *update, Mix_Chunk *deathSound, int *escapePressed, int *gameState, int amAlive, UDPpacket *packet, UDPsocket *client_socket)
{
    if (event->type != SDL_KEYDOWN)
        return;

    SDL_Rect nextPos = *rect;

    switch (event->key.keysym.sym)
    {
    case SDLK_w:
        nextPos.y -= PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && 
            !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound, packet, client_socket) && 
            amAlive)
        {
            rect->y -= PLAYER_MOVE_SPEED;
            *movement = 1;
            // Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players))
                Mix_PlayChannel(-1, coinsSound, 0);
        }

        break;
    case SDLK_s:
        nextPos.y += PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && 
        !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound, packet, client_socket) && 
        amAlive)
        {
            rect->y += PLAYER_MOVE_SPEED;
            *movement = 2;
            // collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players);
            // Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players))
                Mix_PlayChannel(-1, coinsSound, 0);
        }
        break;
    case SDLK_a:
        nextPos.x -= PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && 
        !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound, packet, client_socket) && 
        amAlive)
        {
            rect->x -= PLAYER_MOVE_SPEED;
            *movement = 3;
            // collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players);
            // Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players))
                Mix_PlayChannel(-1, coinsSound, 0);
        }
        break;
    case SDLK_d:
        nextPos.x += PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && 
        !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound, packet, client_socket) && 
        amAlive)
        {
            rect->x += PLAYER_MOVE_SPEED;
            *movement = 4;
            // collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players);
            // Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer, players))
                Mix_PlayChannel(-1, coinsSound, 0);
        }
        break;
    case SDLK_TAB:
        (*gameState) = 5;
       
        break;
    case SDLK_ESCAPE:
        (*gameState) = 4;
        // (*escapePressed) = 1;
        break;
    case SDL_KEYUP:
        Mix_FreeChunk(music);
        Mix_FreeChunk(coinsSound);
        break;
    // case SDLK_UP:
    //     // Move up
    //     break;
    // case SDLK_LEFT:
    //     // Move left
    //     break;
    // case SDLK_DOWN:
    //     // Move down
    //     break;
    // case SDLK_RIGHT:
    //     // Move right
    //     break;
    default:
        break;
    }
}
