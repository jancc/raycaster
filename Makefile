CC ?= gcc
SDL2_CONFIG ?= sdl2-config
CFLAGS := -std=c11 -Wall -Wextra
CFLAGS += $(shell $(SDL2_CONFIG) --cflags)
LIBS ?= $(shell $(SDL2_CONFIG) --libs) -lSDL2_image -lphysfs -lm
PREFIX = /usr/local
SOURCES := $(wildcard src/*.c)
INCLUDE := -Iinclude/
OBJ := $(patsubst %.c,%.o,$(SOURCES))
BIN = raycaster

prog: $(OBJ)
	$(CC) $(CFLAGS) -no-pie $(LIBS) $(INCLUDE) $(OBJ) -o $(BIN)

%.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -DDEBUG -g
debug: prog

release: CFLAGS += -O2
release: prog

format:
	clang-format -i src/*.c include/*.h

clean:
	rm $(OBJ)
	rm $(BIN)

.PHONY: format clean
