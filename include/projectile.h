#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdint.h>

#define PROJECTILES_MAX_COUNT

typedef struct sprite_s Sprite;

typedef struct projectile_s {
    uint8_t active;
    double x;
    double y;
    double vx;
    double vy;
    Sprite* sprite;
    uint8_t isEvil;
} Projectile;

void projectilesUpdate();
uint8_t projectileSpawn(double x, double y, double vx, double vy,
                        Sprite* sprite, uint8_t isEvil);

#endif
