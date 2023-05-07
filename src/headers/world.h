#ifndef world_h
#define world_h
#include <stdio.h>

int getRow(int y);
int getCol(int x);
int getTileRows();
int getTileColumns();
int getTileWidth();
int getTileHeight();
int getTileGrid(int x, int y);

#endif /* world_h */