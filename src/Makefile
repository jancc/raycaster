CC = gcc
INCLUDE = -I../include
CFLAGS = -std=c11 -Wall -Wextra
LIBS = `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lphysfs -lm 
OBJ = gfx.o world.o game.o input.o bmath.o menu.o player.o monster.o timing.o
BIN = ../raycaster

prog: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJ) main.c -o $(BIN) $(LIBS)

debug: CFLAGS += -DDEBUG -g -fsanitize=address -fsanitize=leak
debug: prog

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $<

clean:
	rm *.o
	cd ..
	rm $(BIN)

.PHONY: clean