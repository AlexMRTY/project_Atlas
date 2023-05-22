#ifndef client_h
#define client_h
#include <SDL2/SDL.h>
#include "player.h"
#include "coins.h"

int joinAccept(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement, int *isAlive, int *isHunter);
int recievePlayerData(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement, int *isAlive, int *isHunter);
void printPlayerData(UDPpacket *recieve);
void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket);
void transmitCoins(Coins coins[], int numberOfCoins, UDPpacket *packet, UDPsocket *client_socket, int update);
void transmittDiedPlayer(UDPpacket *packet, UDPsocket *client_socket, int id, int numberOfPlayers, int killerId);
void HandleUDPRecv(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer, Coins coins[], int *numberOfCoins, int *gameOver, int *gameState);
#endif