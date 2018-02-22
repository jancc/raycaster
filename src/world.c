#include "engine.h"

static uint8_t * tilemap;
static uint32_t tilemapWidth;
static uint32_t tilemapHeight;
static Ent ** ents;
static size_t entsArraySize;
static size_t entsCount;

// double the size of our ents array
void entsArrayExpand() {
    ents = realloc(ents, entsArraySize * 2 * sizeof(Ent *));
    for(size_t i = entsArraySize; i < entsArraySize * 2; i++) {
        ents[i] = NULL;
    }
    entsArraySize *= 2;
}

void worldInit() {
    entsArraySize = 1;
    entsCount = 0;
    ents = malloc(entsArraySize * sizeof(Ent *));
    ents[0] = NULL;
}

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

Ent * worldSpawnEnt(double x, double y) {
    Ent * newEnt = calloc(1, sizeof(Ent));
    newEnt->x = x;
    newEnt->y = y;
    if(entsCount + 1 > entsArraySize) {
        entsArrayExpand();
    }
    for(size_t i = 0; i < entsArraySize; i++) {
        if(ents[i] == NULL) {
            ents[i] = newEnt;
            entsCount++;
            return newEnt;
        }
    }
    // should not happen normally
    free(newEnt);
    return NULL;
}

Ent ** worldGetEnts() {
    return ents;
}

size_t worldGetEntsSize() {
    return entsArraySize;
}

/*
 * Adapted from:
 * https://tavianator.com/fast-branchless-raybounding-box-intersections/
*/
uint8_t worldHitscanTest(Ray * ray, AABB * box) {
    double invDX = 1 / ray->dx;
    double invDY = 1 / ray->dy;
    double tx1 = (box->x1 - ray->x)*invDX;
    double tx2 = (box->x2 - ray->x)*invDX;

    double tmin = fmin(tx1, tx2);
    double tmax = fmax(tx1, tx2);

    double ty1 = (box->y1 - ray->y)*invDY;
    double ty2 = (box->y2 - ray->y)*invDY;

    tmin = fmax(tmin, fmin(ty1, ty2));
    tmax = fmin(tmax, fmax(ty1, ty2));

    return tmax >= tmin;
}

void worldClear() {
    free(tilemap);
    tilemapWidth = 0;
    tilemapHeight = 0;
}
