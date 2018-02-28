#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

typedef struct sprite_s Sprite;
typedef struct player_s Player;
typedef struct monster_s Monster;

typedef struct hitscanOut_s {
    uint8_t hit;
    double x;
    double y;
    Monster * monster;
} HitscanOut;

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
void worldResetPlayer(double x, double y);
Player * worldGetPlayer();
Monster * worldSpawnMonster(double x, double y);
void worldDespawnMonster(Monster * monster, uint8_t autoFree);
Monster ** worldGetMonsters();
size_t worldGetMonstersSize();
uint8_t worldHitscan(Ray * ray, HitscanOut * out);
void worldInit();
void worldUpdate();
void worldClear();

#endif
