SRCDIR=./src
DIR_SEP=/

ifeq ($(OS),Windows_NT)
    # Windows-specific libraries
	INCLUDE = -L/opt/homebrew/lib/
    LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net
    RM = rm
else
    # Mac-specific libraries
    INCLUDE = -I/usr/local/include
    LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_net -lSDL2main
    LDFLAGS = -Wl,-rpath,/usr/local/lib
    RM = rm -f
endif

CC=gcc
CFLAGS = -g -c $(INCLUDE)

all: main server

main: main.o world.o
	$(CC) main.o world.o -o main $(LDFLAGS) $(LIBS)

main.o: $(SRCDIR)$(DIR_SEP)main.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)main.c -o main.o

world.o: $(SRCDIR)$(DIR_SEP)world.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)world.c

server: server.o
	$(CC) server.o -o server $(LDFLAGS) $(LIBS)

server.o: $(SRCDIR)$(DIR_SEP)server.c
	$(CC) $(CFLAGS) $(SRCDIR)$(DIR_SEP)server.c -o server.o

clean:
	$(RM) *.o main server