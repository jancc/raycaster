#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"
#include <stdint.h>

typedef struct sprite_s Sprite;

typedef struct player_s {
    double x;
    double y;
    double rot;
    uint8_t health;
    struct weapon_s {
        Sprite* sprite;
        double animationStartTime;
        double maxSqrDistance;
        double damage;
        double frequency;
        double nextShot;
    } weapon;
} Player;

void playerUpdate(Player* player);
void playerSelectWeapon(Player* player, WeaponType type);

#endif
