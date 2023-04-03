# Location of where hello.c is stored in
SRCDIR=./src

# Name of the compiler, GNU GCC in this case
CC=gcc-12

# Any include directories
INCLUDE = -I/usr/local/include/SDL2

# Any flags for the compiler
CFLAGS = -g -c $(INCLUDE)

# Any libraries to link with
LIBS = -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_net

# Extra flags to give to compilers when they are supposed to invoke the linker
#LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -mwindows
LDFLAGS = -lSDL2main -lSDL2

main:
	@echo "Building Hello"
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o $@.o 
	$(CC) main.o -o main $(LDFLAGS) -L $(LIBS)

clean:
	rm -f main.o
	rm -f main
