#include "headers/world.h"
#include "headers/render.h"
#include "headers/coins.h"
#include "headers/globalConst.h"
#include "headers/player.h"

void renderMenuBackdrop(SDL_Renderer *pRenderer) {
    // Backdrop
    SDL_Rect menuRect;
	menuRect.x = 0;
	menuRect.y = 0;
	menuRect.w = WINDOW_WIDTH;
	menuRect.h = WINDOW_HEIGHT;	
    
	SDL_Surface* overlaySurface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0, 0, 0, 0);
    if (overlaySurface == NULL) {
        printf("Failed to create RGB surface: %s\n", SDL_GetError());
    }

	int fillRect = SDL_FillRect(overlaySurface, NULL, SDL_MapRGBA(overlaySurface->format, 0, 0, 0, 128));
    if (fillRect < 0) {
        printf("Failed to fill Rect: %d", fillRect);
    }
    
	SDL_Texture* overlayTexture = SDL_CreateTextureFromSurface(pRenderer, overlaySurface);
    if (overlayTexture == NULL) {
        printf("Failed to create texture from surface 1: %s\n", SDL_GetError());
    }
    
    SDL_RenderCopy(pRenderer, overlayTexture, NULL, &menuRect);
    SDL_FreeSurface(overlaySurface);
    SDL_DestroyTexture(overlayTexture);
}

void renderMap(SDL_Renderer *gRenderer, SDL_Texture *mTiles, SDL_Rect gTiles[])
{
    SDL_Rect position;
    position.y = 0;
    position.x = 0;
    position.h = getTileHeight();
    position.w = getTileWidth();

    for (int i = 0; i < getTileColumns(); i++)
    {
        for (int j = 0; j < getTileRows(); j++)
        {
            position.y = i * getTileHeight();
            position.x = j * getTileWidth();
            SDL_RenderCopyEx(gRenderer, mTiles, &gTiles[getTileGrid(i, j)], &position, 0, NULL, SDL_FLIP_NONE);
        }
    }
}

void renderPlayers(SDL_Renderer *pRenderer, SDL_Texture **pTexture, SDL_Rect *subtextures, int num_subtextures, Player *players, int num_players, Player me, SDL_Texture *ppTexture)
{
    

    // Change current subtexture based on player movement
    SDL_Rect currentSubtexture;
    if (me.isAlive == 1)
    {
        if (me.movement == 1)
        {
            currentSubtexture = subtextures[0];
        }
        else if (me.movement == 3)
        {
            currentSubtexture = subtextures[1];
            SDL_Texture *playerTexture = pTexture[me.id];
            SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
            SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &me.rect, 0, NULL, flip);
        }
        else if (me.movement == 2)
        {
            currentSubtexture = subtextures[2];
        }
        else if (me.movement == 4)
        {
            currentSubtexture = subtextures[1];
        }

        // Render current player with correct subtexture
        SDL_Texture *playerTexture = pTexture[me.id];
        SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &me.rect, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        SDL_RenderCopy(pRenderer, ppTexture, NULL, &me.rect);
    }

    // Render all other players
    for (int i = 0; i < num_players; i++)
    {
        if (players[i].id != me.id)
        {
            if (players[i].isAlive == 1)
            {
                if (players[i].movement == 1)
                {
                    currentSubtexture = subtextures[0];
                }
                else if (players[i].movement == 3)
                {
                    currentSubtexture = subtextures[1];
                    SDL_Texture *playerTexture = pTexture[players[i].id];
                    SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
                    SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &players[i].rect, 0, NULL, flip);
                }
                else if (players[i].movement == 2)
                {
                    currentSubtexture = subtextures[2];
                }
                else if (players[i].movement == 4)
                {
                    currentSubtexture = subtextures[1];
                }

                // Render current player with correct subtexture
                SDL_Texture *playerTexture = pTexture[players[i].id];
                SDL_RenderCopyEx(pRenderer, playerTexture, &currentSubtexture, &players[i].rect, 0, NULL, SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopy(pRenderer, ppTexture, NULL, &players[i].rect);
            }
        }
    }
}

void loadTiles(SDL_Renderer *gRenderer, SDL_Texture **mTiles, SDL_Rect gTiles[])
{
    SDL_Surface *gTilesSurface = IMG_Load("resources/tilemap.png");
    if (!gTilesSurface)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(gRenderer);
        // SDL_DestroyWindow(Window);
        SDL_Quit();
    }
    *mTiles = SDL_CreateTextureFromSurface(gRenderer, gTilesSurface);
    for (int i = 0; i < 8; i++)
    {
        gTiles[i].x = i * getTileWidth();
        gTiles[i].y = 0;
        gTiles[i].w = getTileWidth();
        gTiles[i].h = getTileHeight();
    }
}

void loadCoins(SDL_Renderer *pRenderer, SDL_Texture **pTexture, Coins coins[], int *numCoins, SDL_Rect gCoins[])
{
    SDL_Surface *gTilesSurface = IMG_Load("resources/Full Coinss.png");
    *pTexture = SDL_CreateTextureFromSurface(pRenderer, gTilesSurface);

    int coinWidth = gTilesSurface->w / COIN_FRAMES;
    int coinHeight = gTilesSurface->h;

    for (int i = 0; i < COIN_FRAMES; i++)
    {
        gCoins[i].x = i * coinWidth;
        gCoins[i].y = 0;
        gCoins[i].w = coinWidth;
        gCoins[i].h = coinHeight;
    }
}

void renderCoins(SDL_Renderer *pRenderer, SDL_Texture **pTexture, Coins coins[], int numCoins, SDL_Rect gCoins[], int frame)
{
    for (int i = 0; i < numCoins; i++)
    {
        Coins *coin = &coins[i];
        if (coin->isVisible == 1)
        {
            int frameIndex = (frame / COIN_ANIMATION_DELAY) % COIN_FRAMES;

            SDL_Rect coinFrame = gCoins[frameIndex];
            SDL_RenderCopy(pRenderer, *pTexture, &coinFrame, &coin->coin);
        }
    }
}
