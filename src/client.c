#include "client.h"
#include "player.h"
void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket) 
{
    // Send player position update to server
    sprintf((char *)packet->data, "%d %d %d %d %d", me->rect.x, me->rect.y, me->id, me->numberOfPoints, me->movement);
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(*client_socket, -1, packet);
}