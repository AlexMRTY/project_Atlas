#include "headers/coins.h"
#include "headers/globalConst.h"
#include "headers/world.h"

void initializeCoins(Coins coins[], int *numberOfCoins)
{
    for (int row = 0; row < getTileRows(); row++)
    {
        for (int col = 0; col < getTileColumns(); col++)
        {
            if (getTileGrid(row, col) == 7 && (*numberOfCoins) < MAX_COINS)
            {
                // Add a coin to the center of the tile
                int coinX = col * getTileWidth() + getTileWidth() / 2;
                int coinY = row * getTileHeight() + getTileHeight() / 2;
                coins[(*numberOfCoins)] = (Coins){{coinX, coinY, COINS_MAX_W, COINS_MAX_H}, 1, 1, *numberOfCoins}; // Array of coins
                (*numberOfCoins)++;
            }
        }
    }
}

void updateCoins(Coins coins[], int j, int isvisible)
{
    coins[j].isVisible = isvisible;
}
