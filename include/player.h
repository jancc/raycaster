#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "config.h"

typedef struct player_s {
    double x;
    double y;
    double rot;
    uint8_t health;
    struct weapon_s {
        Sprite * sprite;
        double maxSqrDistance;
        double damage;
        double frequency;
        double nextShot;
    } weapon;
} Player;

void playerUpdate(Player * player);

#endif
