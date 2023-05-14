#include "headers/lobby.h"
#include <SDL2/SDL_ttf.h>
#include "headers/globalConst.h"
#include "headers/render.h"

void waitingForPlayers(SDL_Renderer *pRenderer, TTF_Font* font, int numOfPlayers)
{
    const char* labels[5] = {"Waiting for Players...", "1 of 4", "2 of 4", "3 of 4", "4 of 4"};

    SDL_Color color[1] = {{112, 15, 43}};

    SDL_Rect rects[2];

    
    SDL_Surface* surfaces[2];
    SDL_Texture* textures[2];


    surfaces[0] = TTF_RenderText_Solid(font,labels[0],color[0]);
    surfaces[1] = TTF_RenderText_Solid(font,labels[numOfPlayers],color[0]);

    textures[0] = SDL_CreateTextureFromSurface(pRenderer, surfaces[0]);
    if (textures[0] == NULL) {
        printf("Failed to create texture from surface textures 0: %s\n", SDL_GetError());
    }
    textures[1] = SDL_CreateTextureFromSurface(pRenderer, surfaces[1]);
    if (textures[0] == NULL) {
        printf("Failed to create texture from surface textures 0: %s\n", SDL_GetError());
    }


    SDL_QueryTexture(textures[0], 0, 0, &rects[0].w, &rects[0].h);
    SDL_QueryTexture(textures[1], 0, 0, &rects[1].w, &rects[1].h);


    rects[0].x = WINDOW_WIDTH/2 - rects[0].w/2;
    rects[0].y = (WINDOW_HEIGHT/2 - rects[0].h) - (1.5*rects[0].h);
    
    rects[1].x = WINDOW_WIDTH/2 - rects[1].w/2;
    rects[1].y = WINDOW_HEIGHT/2 - rects[1].h;


    renderMenuBackdrop(pRenderer);
    SDL_RenderCopy(pRenderer, textures[0], NULL, &rects[0]);
    SDL_RenderCopy(pRenderer, textures[1], NULL, &rects[1]);
    
    SDL_FreeSurface(surfaces[0]);
    SDL_FreeSurface(surfaces[1]);
    SDL_DestroyTexture(textures[0]);
    SDL_DestroyTexture(textures[1]);

    SDL_RenderPresent(pRenderer);


    // SDL_Surface* surface = TTF_RenderText_Solid(font,labels[state],color[0]);
    // SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);
    // if (texture == NULL) {
    //     printf("Failed to create texture from surface endgame: %s\n", SDL_GetError());
    // }
    // SDL_QueryTexture(texture, 0, 0, &rect.w, &rect.h);

    // rect.w *= 2;
    // rect.h *= 2;
    // rect.x = WINDOW_WIDTH/2 - rect.w/2;
    // rect.y = WINDOW_HEIGHT/2 - rect.h; 
    

    // // renderMenuBackdrop(pRenderer);
    // SDL_RenderCopy(pRenderer, texture, NULL, &rect);
}