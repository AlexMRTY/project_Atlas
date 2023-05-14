#include "headers/start.h"
#include <SDL2/SDL_ttf.h>
#include "headers/globalConst.h"

#include <stdbool.h>

int startMenu(SDL_Renderer *pRenderer, bool *quit, TTF_Font *font)
{
    int x, y;
    const char *labels[GAMEMENUOPTIONS] = {"Start A New Game", "Exit"};
    SDL_Surface *menus[GAMEMENUOPTIONS];
    int selected = 0;
    SDL_Color color[2] = {{255, 255, 255}, {255, 0, 0}};
    SDL_Rect pos[GAMEMENUOPTIONS];

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
    menus[0] = TTF_RenderText_Solid(font, labels[0], color[1]);
    menus[1] = TTF_RenderText_Solid(font, labels[1], color[0]);

    // Create texture from each surface
    SDL_Texture *newGameTexture = SDL_CreateTextureFromSurface(pRenderer, menus[0]);
    if (newGameTexture == NULL)
    {
        printf("Failed to create texture from surface 2: %s\n", SDL_GetError());
    }
    SDL_Texture *exitTexture = SDL_CreateTextureFromSurface(pRenderer, menus[1]);
    if (exitTexture == NULL)
    {
        printf("Failed to create texture from surface 3: %s\n", SDL_GetError());
    }

    // Get the dimensions of each texture
    SDL_QueryTexture(newGameTexture, 0, 0, &pos[0].w, &pos[0].h);
    SDL_QueryTexture(exitTexture, 0, 0, &pos[1].w, &pos[1].h);

    // Get the positions
    pos[0].x = WINDOW_WIDTH / 2 - pos[0].w / 2;
    pos[0].y = WINDOW_HEIGHT / 2 - pos[0].h;

    pos[1].x = WINDOW_WIDTH / 2 - pos[1].w / 2;
    pos[1].y = WINDOW_HEIGHT / 2 + pos[1].h + 30;

    // Render the textures onto the screen
    SDL_RenderCopy(pRenderer, newGameTexture, NULL, &pos[0]);
    SDL_RenderCopy(pRenderer, exitTexture, NULL, &pos[1]);

    SDL_Event event;

    SDL_Texture *temp1;
    SDL_Texture *temp2;

    while (!(*quit))
    {
        SDL_PollEvent(&event);

        *quit = event.type == SDL_QUIT ? true : false;

        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            selected = 1;
            activeMenu(pRenderer, menus, color, pos, temp1, temp2, font, selected);
            break;
        case SDLK_DOWN:
            selected = 2;
            activeMenu(pRenderer, menus, color, pos, temp1, temp2, font, selected);
            break;
        case SDLK_RETURN:
            if (selected == 1)
            {
                for (int i = 0; i < GAMEMENUOPTIONS; i++)
                {
                    SDL_FreeSurface(menus[i]);
                }
                return selected;
            }
            else if (selected == 2)
            {
                *quit = true;
            }
            break;
        default:
            break;
        }
        SDL_RenderClear(pRenderer);
        activeMenu(pRenderer, menus, color, pos, temp1, temp2, font, selected);
        SDL_RenderPresent(pRenderer);
    }
    return selected;
}

void activeMenu(SDL_Renderer *pRenderer, SDL_Surface *menus[], SDL_Color color[], SDL_Rect pos[], SDL_Texture *temp1, SDL_Texture *temp2, TTF_Font *font, int selected)
{
    SDL_RenderClear(pRenderer);

    menus[0] = TTF_RenderText_Solid(font, "Start New Game", color[selected == 1 ? 1 : 0]);
    menus[1] = TTF_RenderText_Solid(font, "Exit", color[selected == 2 ? 1 : 0]);
    temp1 = SDL_CreateTextureFromSurface(pRenderer, menus[0]);
    temp2 = SDL_CreateTextureFromSurface(pRenderer, menus[1]);
    SDL_RenderCopy(pRenderer, temp1, NULL, &pos[0]);
    SDL_RenderCopy(pRenderer, temp2, NULL, &pos[1]);
}