#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>

typedef struct sprite_s Sprite;

typedef enum monster_type_e {
    MT_theUltimateMan
} MonsterType;

typedef struct monster_s {
    double x;
    double y;
    Sprite * sprite;
    MonsterType type;
} Monster;

void monstersUpdate();

#endif
