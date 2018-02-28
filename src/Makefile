CC = clang
INCLUDE = -I../include
CFLAGS = -std=c11 -Wall -Wextra
LIBS = `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lm 
OBJ = gfx.o world.o game.o input.o bmath.o menu.o player.o monster.o
BIN = ../raycaster

prog: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) main.c -o $(BIN) $(LIBS)

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $<

clean:
	rm *.o
	cd ..
	rm $(BIN)

.PHONY: clean