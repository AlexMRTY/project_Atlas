SRCDIR=./src
DIR_SEP=/

ifeq ($(OS),Windows_NT)
    # Windows-specific libraries
	INCLUDE = -L/opt/homebrew/lib/
    LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer -lSDL2_ttf

    RM = rm
else
    # Mac-specific libraries
    INCLUDE = -I/usr/local/include
    LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_net -lSDL2main -lSDL2_mixer -lSDL2_ttf

    LDFLAGS = -Wl,-rpath,/usr/local/lib
    RM = rm -f
endif

CC=gcc
CFLAGS = -g -c $(INCLUDE)

MAINDEPENDENCIES=main.o world.o collisionDetection.o events.o render.o client.o player.o pause.o endgame.o lobby.o start.o scoreList.o
SERVERDEPENDENCIES=server.o world.o player.o collisionDetection.o coins.o

all: main server

main: $(MAINDEPENDENCIES) 
	$(CC) $(MAINDEPENDENCIES) -o main $(LDFLAGS) $(LIBS)

main.o: $(SRCDIR)$(DIR_SEP)main.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)main.c -o main.o

world.o: $(SRCDIR)$(DIR_SEP)world.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)world.c

server: $(SERVERDEPENDENCIES)
	$(CC) $(SERVERDEPENDENCIES) -o server $(LDFLAGS) $(LIBS)
	$(RM) *.o

server.o: $(SRCDIR)$(DIR_SEP)server.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)server.c -o server.o

collisionDetection.o: $(SRCDIR)$(DIR_SEP)collisionDetection.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)collisionDetection.c

events.o: $(SRCDIR)$(DIR_SEP)events.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)events.c

render.o: $(SRCDIR)$(DIR_SEP)render.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)render.c

client.o: $(SRCDIR)$(DIR_SEP)client.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)client.c

player.o: $(SRCDIR)$(DIR_SEP)player.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)player.c

coins.o: $(SRCDIR)$(DIR_SEP)coins.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)coins.c	

pause.o: $(SRCDIR)$(DIR_SEP)pause.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)pause.c	

endgame.o: $(SRCDIR)$(DIR_SEP)endgame.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)endgame.c	

lobby.o: $(SRCDIR)$(DIR_SEP)lobby.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)lobby.c	

start.o: $(SRCDIR)$(DIR_SEP)start.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)start.c	

scoreList.o: $(SRCDIR)$(DIR_SEP)scoreList.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)scoreList.c

clean:
	$(RM) *.o *.exe main server