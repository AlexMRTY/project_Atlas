SRCDIR=./src

CC=gcc-12

INCLUDE = -I/usr/local/include

CFLAGS = -g -c $(INCLUDE)

LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_net -lSDL2main

LDFLAGS = -Wl,-rpath,/usr/local/lib

all: main server

main: main.o
	$(CC) main.o -o main $(LDFLAGS) $(LIBS)

main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o main.o

server: server.o
	$(CC) server.o -o server $(LDFLAGS) $(LIBS)

server.o: $(SRCDIR)/server.c
	$(CC) $(CFLAGS) $(SRCDIR)/server.c -o server.o

clean:
	rm -f *.o
	rm -f main
	rm -f server
