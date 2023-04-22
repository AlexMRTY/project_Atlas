#include "world.h"
#include "render.h"

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

void renderPlayers(SDL_Renderer *pRenderer, SDL_Texture **pTexture, SDL_Rect *subtextures, int num_subtextures, Player *players, int num_players, Player me)
{
    // Change current subtexture based on player movement
    SDL_Rect currentSubtexture;
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

    // Render all other players
    for (int i = 0; i < num_players; i++)
    {
        if (players[i].id != me.id)
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
    }

    SDL_RenderPresent(pRenderer);
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