#include "globalConst.h"
#include "collisionDetection.h"
#include "events.h"
#include "SDL2/SDL_mixer.h"

void handleEvents(SDL_Rect *rect, int *movement, bool *quit, Mix_Chunk *music)
{

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        *quit = handleQuit(&event);

        transformCharacter(&event, rect, movement, music);
    }
}

bool handleQuit(SDL_Event *event)
{
    return (event->type == SDL_QUIT) ? true : false;
}

void transformCharacter(SDL_Event *event, SDL_Rect *rect, int *movement, Mix_Chunk *music)
{
    if (event->type != SDL_KEYDOWN)
        return;

    switch (event->key.keysym.sym)
    {
    case SDLK_UP:

        rect->y = (!collisionWithWall(rect->x, rect->y - PLAYER_MOVE_SPEED))
                      ? (rect->y - PLAYER_MOVE_SPEED)
                      : rect->y; // Ternary operator alternative

        *movement = 1; // Character facing direction
        Mix_PlayChannel(-1, music, 0);
        break;
    case SDLK_DOWN:
        rect->y = (!collisionWithWall(rect->x, rect->y + PLAYER_MOVE_SPEED))
                      ? (rect->y + PLAYER_MOVE_SPEED)
                      : rect->y; // Ternary operator alternative

        *movement = 2; // Character facing direction
        Mix_PlayChannel(-1, music, 0);
        break;
    case SDLK_LEFT:
        rect->x = (!collisionWithWall(rect->x - PLAYER_MOVE_SPEED, rect->y))
                      ? (rect->x - PLAYER_MOVE_SPEED)
                      : rect->x; // Ternary operator alternative
        Mix_PlayChannel(-1, music, 0);
        *movement = 3; // Character facing direction
        break;
    case SDLK_RIGHT:
        rect->x = (!collisionWithWall(rect->x + PLAYER_MOVE_SPEED, rect->y))
                      ? (rect->x + PLAYER_MOVE_SPEED)
                      : rect->x; // Ternary operator alternative

        *movement = 4; // Character facing direction
        Mix_PlayChannel(-1, music, 0);
        break;
    case SDL_KEYUP:
        Mix_FreeChunk(music);
        break;
    default:
        break;
    }
}