//
//  world.c
//
//  Created by Jonas Willén on 2021-03-29.
//

#include "world.h"

static int TILE_ROWS = 32;
static int TILE_COLUMNS = 32;
static int TILE_WIDTH = 32;
static int TILE_HEIGHT = 32;

int getTileRows(){
    return TILE_ROWS;
}

int getTileColumns(){
    return TILE_COLUMNS;
}

int getTileWidth(){
    return TILE_WIDTH;
}

int getTileHeight(){
    return TILE_HEIGHT;
}

// int getTileGrid(int x, int y){
//     return tileGrid[x][y];
// }