#include "headers/scoreList.h"
#include <SDL2/SDL_ttf.h>
#include "headers/globalConst.h"

#include <stdbool.h>

void ScoreListMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font)
{
    int x, y;
    const char *scores[4] = {"1000", "800", "500", "200"};
    SDL_Surface *menus[4];
    SDL_Color color = {255, 255, 255};
    SDL_Rect pos[4];

    // Backdrop
    SDL_Rect menuRect;
    menuRect.x = 0;
    menuRect.y = 0;
    menuRect.w = WINDOW_WIDTH;
    menuRect.h = WINDOW_HEIGHT;

    SDL_Surface *overlaySurface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0, 0, 0, 0);
    if (overlaySurface == NULL)
    {
        printf("Failed to create RGB surface: %s\n", SDL_GetError());
    }

    int fillRect = SDL_FillRect(overlaySurface, NULL, SDL_MapRGBA(overlaySurface->format, 0, 0, 0, 128));
    if (fillRect < 0)
    {
        printf("Failed to fill Rect: %d", fillRect);
    }

    SDL_Texture *overlayTexture = SDL_CreateTextureFromSurface(pRenderer, overlaySurface);
    if (overlayTexture == NULL)
    {
        printf("Failed to create texture from surface 1: %s\n", SDL_GetError());
    }

    SDL_RenderCopy(pRenderer, overlayTexture, NULL, &menuRect);

    // Menu
    menus[0] = TTF_RenderText_Solid(font, scores[0], color);
    menus[1] = TTF_RenderText_Solid(font, scores[1], color);
    menus[2] = TTF_RenderText_Solid(font, scores[2], color);
    menus[3] = TTF_RenderText_Solid(font, scores[3], color);

    // Create texture from each surface
    SDL_Texture *scoreOne = SDL_CreateTextureFromSurface(pRenderer, menus[0]);
    if (scoreOne == NULL)
    {
        printf("Failed to create texture from surface 2: %s\n", SDL_GetError());
    }
    SDL_Texture *scoreTwo = SDL_CreateTextureFromSurface(pRenderer, menus[1]);
    if (scoreTwo == NULL)
    {
        printf("Failed to create texture from surface 3: %s\n", SDL_GetError());
    }
    SDL_Texture *scoreThree = SDL_CreateTextureFromSurface(pRenderer, menus[2]);
    if (scoreThree == NULL)
    {
        printf("Failed to create texture from surface 3: %s\n", SDL_GetError());
    }
    SDL_Texture *scoreFour = SDL_CreateTextureFromSurface(pRenderer, menus[3]);
    if (scoreFour == NULL)
    {
        printf("Failed to create texture from surface 3: %s\n", SDL_GetError());
    }

    // Get the dimensions of each texture
    SDL_QueryTexture(scoreOne, 0, 0, &pos[0].w, &pos[0].h);
    SDL_QueryTexture(scoreTwo, 0, 0, &pos[1].w, &pos[1].h);
    SDL_QueryTexture(scoreThree, 0, 0, &pos[2].w, &pos[2].h);
    SDL_QueryTexture(scoreFour, 0, 0, &pos[3].w, &pos[3].h);

    // Get the positions
    pos[0].x = WINDOW_WIDTH / 2 - pos[0].w / 2;
    pos[0].y = WINDOW_HEIGHT / 2 - pos[0].h;

    pos[1].x = WINDOW_WIDTH / 2 - pos[1].w / 2;
    pos[1].y = WINDOW_HEIGHT / 2 + pos[1].h + 15;

    pos[2].x = WINDOW_WIDTH / 2 - pos[2].w / 2;
    pos[2].y = WINDOW_HEIGHT / 2 + pos[2].h + 30;

    pos[3].x = WINDOW_WIDTH / 2 - pos[3].w / 2;
    pos[3].y = WINDOW_HEIGHT / 2 + pos[3].h + 45;

    SDL_RenderClear(pRenderer);
    SDL_Texture *temp1 = SDL_CreateTextureFromSurface(pRenderer, menus[0]);
    SDL_Texture *temp2 = SDL_CreateTextureFromSurface(pRenderer, menus[1]);
    SDL_Texture *temp3 = SDL_CreateTextureFromSurface(pRenderer, menus[2]);
    SDL_Texture *temp4 = SDL_CreateTextureFromSurface(pRenderer, menus[3]);
    SDL_RenderCopy(pRenderer, temp1, NULL, &pos[0]);
    SDL_RenderCopy(pRenderer, temp2, NULL, &pos[1]);
    SDL_RenderCopy(pRenderer, temp3, NULL, &pos[2]);
    SDL_RenderCopy(pRenderer, temp4, NULL, &pos[3]);
    SDL_RenderPresent(pRenderer);
}