#include "SDL2/SDL_mixer.h"
#include "headers/globalConst.h"
#include "headers/collisionDetection.h"
#include "headers/events.h"
#include "headers/coins.h"

void handleEvents(SDL_Rect *rect, int *movement, bool *quit, Mix_Chunk *music, Player players[], int currentPlayer, int *nrOfPlayers, int *numberOfPoints, Coins coins[], Mix_Chunk *coinsSound, int *update, Mix_Chunk *deathSound, int *escapePressed)
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        // printf("pollevent: %d", SDL_PollEvent(&event));
        *quit = handleQuit(&event);

        transformCharacter(&event, rect, movement, music, players, currentPlayer, nrOfPlayers, numberOfPoints, coins, coinsSound, update, deathSound, escapePressed);
    }
}

bool handleQuit(SDL_Event *event)
{
    return (event->type == SDL_QUIT) ? true : false;
}

void transformCharacter(SDL_Event *event, SDL_Rect *rect, int *movement, Mix_Chunk *music, Player players[], int currentPlayer, int *nrOfPlayers, int *numberOfPoints, Coins coins[], Mix_Chunk *coinsSound, int *update, Mix_Chunk *deathSound, int *escapePressed)
{
    if (event->type != SDL_KEYDOWN)
        return;

    SDL_Rect nextPos = *rect;

    switch (event->key.keysym.sym)
    {
    case SDLK_w:
        nextPos.y -= PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound))
        {
            rect->y -= PLAYER_MOVE_SPEED;
            *movement = 1;
            Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer))
                Mix_PlayChannel(-1, coinsSound, 0);
        }

        break;
    case SDLK_s:
        nextPos.y += PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound))
        {
            rect->y += PLAYER_MOVE_SPEED;
            *movement = 2;
            Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer))
                Mix_PlayChannel(-1, coinsSound, 0);
        }
        break;
    case SDLK_a:
        nextPos.x -= PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound))
        {
            rect->x -= PLAYER_MOVE_SPEED;
            *movement = 3;
            Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer))
                Mix_PlayChannel(-1, coinsSound, 0);
        }
        break;
    case SDLK_d:
        nextPos.x += PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos, deathSound))
        {
            rect->x += PLAYER_MOVE_SPEED;
            *movement = 4;
            Mix_PlayChannel(-1, music, 0);
            if (collisionWithCoins(coins, numberOfPoints, rect, update, currentPlayer))
                Mix_PlayChannel(-1, coinsSound, 0);
        }
        break;
    case SDL_KEYUP:
        Mix_FreeChunk(music);
        Mix_FreeChunk(coinsSound);
        break;
    case SDLK_ESCAPE:
        (*escapePressed) = 1;
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
