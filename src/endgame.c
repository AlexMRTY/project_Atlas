#include "headers/endgame.h"
#include <SDL2/SDL_ttf.h>
#include "headers/globalConst.h"
#include "headers/render.h"

void endGame(SDL_Renderer *pRenderer, TTF_Font* font, int state)
{
    const char* labels[2] = {"You died", "You Won"};

    SDL_Color color[2] = {{112, 15, 43},{255,0,0}};

    SDL_Rect rect;
    SDL_Surface* surface = TTF_RenderText_Solid(font,labels[state],color[0]);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);
    if (texture == NULL) {
        printf("Failed to create texture from surface endgame: %s\n", SDL_GetError());
    }
    SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);

    rect.w *= 2;
    rect.h *= 2;
    rect.x = WINDOW_WIDTH/2 - rect.w/2;
    rect.y = WINDOW_HEIGHT/2 - rect.h; 
    

    SDL_RenderCopy(pRenderer, texture, NULL, &rect);
}