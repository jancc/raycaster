#include "engine.h"

static Projectile * projectiles;

void projectilesInit() {
    projectiles = malloc(sizeof(Projectile) * PROJECTILES_MAX_COUNT);
    memset(projectiles, 0, sizeof(Projectile) * PROJECTILES_MAX_COUNT);
}

void projectilesUpdate() {
    for(size_t i = 0; i < PROJECTILES_MAX_COUNT; i++) {
        
    }
}

uint8_t projectileSpawn(double x, double y, double vx, double vy, Sprite * sprite, uint8_t isEvil) {
    for(size_t i = 0; i < PROJECTILES_MAX_COUNT; i++) {
    }
}
