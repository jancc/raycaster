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

uint8_t worldGetCollision(uint32_t x, uint32_t y) {
    return worldGetTile(x, y) != 0;
}

uint8_t worldGetCollisionInArea(double areaX, double areaY, double areaW, double areaH) {
    uint32_t xStart = (uint32_t)areaX;
    uint32_t yStart = (uint32_t)areaY;
    uint32_t xEnd = (uint32_t)(areaX + areaW);
    uint32_t yEnd = (uint32_t)(areaY + areaH);
    for(uint32_t y = yStart; y <= yEnd; y++) {
        for(uint32_t x = xStart; x <= xEnd; x++) {
            if(worldGetCollision(x, y)) return 1;
        }
    }
    return 0;
}

void worldClear() {
    free(tilemap);
    tilemapWidth = 0;
    tilemapHeight = 0;
}