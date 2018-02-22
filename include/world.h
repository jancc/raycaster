#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

typedef struct sprite_s Sprite;

typedef struct ent_s {
    double x;
    double y;
    Sprite * sprite;
} Ent;

typedef struct hitcastOut_s {
    uint8_t hit;
    Ent * ent;
} HitcastOut;

typedef struct ray_s {
    double x;
    double y;
    double dx;
    double dy;
} Ray;

typedef struct aabb_s {
    double x1;
    double y1;
    double x2;
    double y2;
} AABB;

void worldCreateTilemap(uint32_t width, uint32_t height);
void worldRandomize();
void worldSetTile(uint32_t x, uint32_t y, uint8_t value);
uint8_t worldGetTile(uint32_t x, uint32_t y);
uint8_t worldGetCollision(uint32_t x, uint32_t y);
uint8_t worldGetCollisionInArea(double x, double y, double w, double h);
Ent * worldSpawnEnt(double x, double y);
Ent ** worldGetEnts();
size_t worldGetEntsSize();
HitcastOut worldHitcast(double x, double y, double dx, double dy);
uint8_t worldHitscanTest(Ray * ray, AABB * box);
void worldInit();
void worldUpdate();
void worldClear();

#endif
