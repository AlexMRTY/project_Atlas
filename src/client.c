#include "client.h"
#include "player.h"
#include "globalConst.h"
#include "SDL2/SDL_net.h"

void transmitData(Player *me, UDPpacket *packet, UDPsocket *client_socket)
{
    // Send player position update to server
    sprintf((char *)packet->data, "%d %d %d %d %d", me->rect.x, me->rect.y, me->id, me->numberOfPoints, me->movement);
    packet->len = strlen((char *)packet->data) + 1;
    SDLNet_UDP_Send(*client_socket, -1, packet);
}

void receive_updates(UDPpacket *recieve, Player players[MAX_PLAYERS], int *number_of_player, bool *joinedServer)
{

    int x2, y2, id2, nrOfpoints_2, movement_2;
    if (sscanf((char *)recieve->data, "player_data %d %d %d %d %d", &x2, &y2, &id2, &nrOfpoints_2, &movement_2) == 5)
    {
        printf("data from other players\n");
        int index = -1;

        if (*number_of_player < MAX_PLAYERS)
        {
            bool found = false;
            for (int i = 0; i < *number_of_player; i++)
            {
                if (players[i].id == id2)
                {
                    found = true;
                    index = i;
                    break;
                }
            }
            if (!found)
            {
                addToTheList(id2, x2, y2, movement_2, nrOfpoints_2, &number_of_player, players);
                printf("Added player with ID %d\n", id2);
            }
            else
            {
                movePlayer(&players[index], x2, y2, movement_2, nrOfpoints_2);
                printf("Player Already exist\n");
            }
        }
    }
    else
    {
        printf("Unknown message received: %s\n", (char *)recieve->data);
    }
    SDLNet_FreePacket(recieve);
}
