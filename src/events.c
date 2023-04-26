#include "globalConst.h"
#include "collisionDetection.h"
#include "events.h"
#include "SDL2/SDL_mixer.h"

void handleEvents(SDL_Rect *rect, int *movement, bool *quit, Mix_Chunk *music, Player players[], int currentPlayer, int nrOfPlayers)
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        *quit = handleQuit(&event);

        transformCharacter(&event, rect, movement, music, players, currentPlayer, nrOfPlayers);
    }
}

bool handleQuit(SDL_Event *event)
{
    return (event->type == SDL_QUIT) ? true : false;
}

void transformCharacter(SDL_Event *event, SDL_Rect *rect, int *movement, Mix_Chunk *music, Player players[], int currentPlayer, int nrOfPlayers)
{
    if (event->type != SDL_KEYDOWN)
        return;

    SDL_Rect nextPos = *rect;

    switch (event->key.keysym.sym)
    {
    case SDLK_UP:
        nextPos.y -= PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos))
        {
            rect->y -= PLAYER_MOVE_SPEED;
            *movement = 1;
            Mix_PlayChannel(-1, music, 0);
        }
        break;
    case SDLK_DOWN:
        nextPos.y += PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos))
        {
            rect->y += PLAYER_MOVE_SPEED;
            *movement = 2;
            Mix_PlayChannel(-1, music, 0);
        }
        break;
    case SDLK_LEFT:
        nextPos.x -= PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos))
        {
            rect->x -= PLAYER_MOVE_SPEED;
            *movement = 3;
            Mix_PlayChannel(-1, music, 0);
        }
        break;
    case SDLK_RIGHT:
        nextPos.x += PLAYER_MOVE_SPEED;
        if (!collisionWithWall(nextPos.x, nextPos.y) && !collisionWithPlayer(players, currentPlayer, nrOfPlayers, &nextPos))
        {
            rect->x += PLAYER_MOVE_SPEED;
            *movement = 4;
            Mix_PlayChannel(-1, music, 0);
        }
        break;
    case SDL_KEYUP:
        Mix_FreeChunk(music);
        break;
    default:
        break;
    }
}
