#ifndef lobby_h
#define lobby_h

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "SDL2/SDL_net.h"
#include <SDL2/SDL_mixer.h>

#include "globalConst.h"
#include <SDL2/SDL_ttf.h>
#include "player.h"
#include "coins.h"
#include "client.h"
#include "events.h"

void waitingForPlayers(SDL_Renderer *pRenderer, TTF_Font* font, int numOfPlayers);
void lobby(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numCoins, SDL_Renderer *pRenderer, TTF_Font *font, bool *quit, Mix_Chunk *music, Mix_Chunk *coinsSound, Mix_Chunk *deathSound, int *update, int *escapePressed, int *gameState, int *gameOver);

#endif