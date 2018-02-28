#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

typedef struct player_s {
    double x;
    double y;
    double rot;
    uint8_t health;
} Player;

void playerUpdate(Player * player);

#endif
