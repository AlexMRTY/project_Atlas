SRCDIR=./src
DIR_SEP=/

ifeq ($(OS),Windows_NT)
    # Windows-specific libraries
	INCLUDE = -L/opt/homebrew/lib/
    LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer
    RM = rm
else
    # Mac-specific libraries
    INCLUDE = -I/usr/local/include
    LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_net -lSDL2main -lSDL2_mixer
    LDFLAGS = -Wl,-rpath,/usr/local/lib
    RM = rm -f
endif

CC=gcc
CFLAGS = -g -c $(INCLUDE)

all: main server

main: main.o world.o collisionDetection.o events.o render.o client.o player.o
	$(CC) main.o world.o collisionDetection.o events.o render.o client.o player.o -o main $(LDFLAGS) $(LIBS)

main.o: $(SRCDIR)$(DIR_SEP)main.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)main.c -o main.o

world.o: $(SRCDIR)$(DIR_SEP)world.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)world.c

server: server.o world.o collisionDetection.o
	$(CC) server.o world.o player.o collisionDetection.o -o server $(LDFLAGS) $(LIBS)

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

clean:
	$(RM) *.o main server