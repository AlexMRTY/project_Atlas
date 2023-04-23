#ifndef client_h
#define client_h
#include <SDL2/SDL.h>
#include "player.h"

int joinAccept(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement);
int recievePlayerData(UDPpacket *recieve, int *x, int *y, int *id, int *nrOfPoints, int *movement);
void printPlayerData(UDPpacket *recieve);
void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket);
void HandleUDPRecv(UDPsocket *client_socket, UDPpacket *recieve, UDPpacket *packet, Player players[], Player *me, int *number_of_player, bool *joinedServer);
#endif