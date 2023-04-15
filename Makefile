# Makefile for Windows
SRCDIR=./src
CC=gcc
INCLUDE = C:\msys64\mingw64\include\SDL2

CFLAGS = -g -I $(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -mwindows -lm

game: main.o world.o
	$(CC) main.o world.o -o game $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c

world.o: $(SRCDIR)/world.c
	$(CC) $(CFLAGS) $(SRCDIR)/world.c

clean:
	rm *.o
	rm *.exe