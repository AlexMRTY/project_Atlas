#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "world.h"

#define SPEED 200 // 100
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]);
void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]);


int main(int argv, char **args)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }

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

    // SDL_Surface *pSurface = IMG_Load("resources/character.png");
    // if (!pSurface)
    // {
    //     printf("Error: %s\n", SDL_GetError());
    //     SDL_DestroyRenderer(pRenderer);
    //     SDL_DestroyWindow(pWindow);
    //     SDL_Quit();
    //     return 1;
    // }

    // SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
    // SDL_FreeSurface(pSurface);
    // if (!pTexture)
    // {
    //     printf("Error: %s\n", SDL_GetError());
    //     SDL_DestroyRenderer(pRenderer);
    //     SDL_DestroyWindow(pWindow);
    //     SDL_Quit();
    //     return 1;
    // }

    // Character

    SDL_Surface *pCharacterSurface = IMG_Load("resources/character.png");
    if (!pCharacterSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *tCharacter = SDL_CreateTextureFromSurface(pRenderer, pCharacterSurface);
    if (!tCharacter)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return 1;
    }

    // Background
    SDL_Texture *tTiles = NULL;
    SDL_Rect gTiles[8];

    loadTiles(pRenderer, &tTiles, gTiles);

    SDL_Rect characterRect;
    SDL_QueryTexture(tCharacter, NULL, NULL, &characterRect.w, &characterRect.h);
    characterRect.w *= 1.5;
    characterRect.h *= 1.5;
    float characterX = (WINDOW_WIDTH - characterRect.w) / 2;  // left side
    float characterY = (WINDOW_HEIGHT - characterRect.h) / 2; // upper side
    float characterVelocityX = 0;                        // unit: pixels/s
    float characterVelocityY = 0;

    bool closeWindow = false;
    bool up, down, left, right;
    up = down = left = right = false;

    while (!closeWindow)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                closeWindow = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = true;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = true;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = true;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = true;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = false;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = false;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = false;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = false;
                    break;
                }
                break;
            }
        }

        characterVelocityX = characterVelocityY = 0;
        if (up && !down)
            characterVelocityY = -SPEED;
        if (down && !up)
            characterVelocityY = SPEED;
        if (left && !right)
            characterVelocityX = -SPEED;
        if (right && !left)
            characterVelocityX = SPEED;
        characterX += characterVelocityX / 60; // 60 frames/s
        characterY += characterVelocityY / 60;
        if (characterX < 0)
            characterX = 0;
        if (characterY < 0)
            characterY = 0;
        if (characterX > WINDOW_WIDTH - characterRect.w)
            characterX = WINDOW_WIDTH - characterRect.w;
        if (characterY > WINDOW_HEIGHT - characterRect.h)
            characterY = WINDOW_HEIGHT - characterRect.h;
        characterRect.x = characterX;
        characterRect.y = characterY;

        SDL_RenderClear(pRenderer);
        renderMap(pRenderer, tTiles, gTiles);
        SDL_RenderCopy(pRenderer, tCharacter, NULL, &characterRect);
        SDL_RenderPresent(pRenderer);
        SDL_Delay(1000 / 60); // 60 frames/s
    }

    SDL_DestroyTexture(tCharacter);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);

    SDL_Quit();
    return 0;
}

void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[]) {
    SDL_Surface* gTilesSurface = IMG_Load("resources/tilemap.png");
    if (!gTilesSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(gRenderer);
        // SDL_DestroyWindow(Window);
        SDL_Quit();
    }
    *mTiles = SDL_CreateTextureFromSurface(gRenderer, gTilesSurface);
    for (int i = 0; i < 8; i++) {
        gTiles[i].x = i*getTileWidth();
        gTiles[i].y = 0;
        gTiles[i].w = getTileWidth();
        gTiles[i].h = getTileHeight();
    }
}

void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[]){
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();
    
    for (int i = 0; i<getTileColumns(); i++) {
        for (int j = 0; j<getTileRows(); j++) {
            position.y = i*getTileHeight();
            position.x = j*getTileWidth();
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i, j)],&position , 0, NULL, SDL_FLIP_NONE);
        }
    }
}