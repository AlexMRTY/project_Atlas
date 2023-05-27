#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_net.h"
#include <SDL2/SDL_mixer.h>

#include "headers/globalConst.h"
#include <SDL2/SDL_ttf.h>
#include "headers/player.h"
#include "headers/coins.h"
#include "headers/client.h"
#include "headers/events.h"
#include "headers/render.h"
#include "headers/endgame.h"

void playGame(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numCoins, SDL_Renderer *pRenderer, TTF_Font *font, bool *quit, Mix_Chunk *music, Mix_Chunk *coinsSound, Mix_Chunk *deathSound, int *update, int *escapePressed, int *gameState, SDL_Texture *tTiles, SDL_Rect gTiles[], SDL_Texture *tCoins, SDL_Rect gCoins[], int frame, SDL_Texture *client_textures[], SDL_Rect subtextures[], SDL_Texture *ppTexture, int *gameOver)
{
    HandleUDPRecv(client_socket, recieve, packet, players, me, number_of_player, joinedServer, coins, numCoins, gameOver, gameState);

    if (!joinedServer) return;

    // Handles quit and movement events
    handleEvents(&me->rect, &me->movement, quit, music, players, me->id, number_of_player, &me->numberOfPoints, coins, coinsSound, update, deathSound, escapePressed, me->isAlive, gameState, packet, client_socket);

    // Transmit cordinates data to server
    transmitData(me, packet, client_socket);

    // Transmit coins data to server
    transmitCoins(coins, *numCoins, packet, client_socket, *update);

    // if (me->isHunter) 
    // {
    //     transmittDiedPlayer(packet, client_socket, players, *number_of_player);
    // }

    // Clear Window
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(pRenderer);

    // Render background
    renderMap(pRenderer, tTiles, gTiles);

    // Render all coins
    renderCoins(pRenderer, &tCoins, coins, *numCoins, gCoins, frame);

    // Render all players
    renderPlayers(pRenderer, client_textures, subtextures, NUM_SUBTEXTURES, players, *number_of_player, *me, ppTexture);

    // TESTING GAMEOVER/VICTORY
    if (!me->isAlive) {
        endGame(pRenderer, font, 0);
    }
    
    // Render frame
    SDL_RenderPresent(pRenderer); 
}