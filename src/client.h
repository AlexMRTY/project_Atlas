#include <SDL2/SDL.h>
#include "player.h"
#include "SDL2/SDL_net.h"
#include <stdbool.h>
#include "globalConst.h"

void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket);
void receive_updates(UDPpacket *recieve, Player players[], int *number_of_player, bool *joinedServer);
