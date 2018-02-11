#include "engine.h"

static uint8_t * tilemap;
static uint32_t tilemapWidth;
static uint32_t tilemapHeight;

void worldCreateTilemap(uint32_t width, uint32_t height) {
    tilemap = calloc(width * height, sizeof(uint8_t));
    tilemapWidth = width;
    tilemapHeight = height;
}

void worldRandomize() {
    for(uint32_t y = 0; y < tilemapHeight; y++) {
        for(uint32_t x = 0; x < tilemapWidth; x++) {
            if(y == 0 || y == tilemapHeight - 1 || x == 0 || x == tilemapWidth - 1) {
                worldSetTile(x, y, 1);
            } else if (rand() % 100 > 95) {
                worldSetTile(x, y, 1 + rand() % 7);
            }
        }
    }
}

void worldSetTile(uint32_t x, uint32_t y, uint8_t value) {
    tilemap[y * tilemapHeight + x] = value;
}

uint8_t worldGetTile(uint32_t x, uint32_t y) {
    return tilemap[y * tilemapHeight + x];
}

void worldClear() {
    free(tilemap);
    tilemapWidth = 0;
    tilemapHeight = 0;
}