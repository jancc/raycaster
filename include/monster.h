#ifndef MONSTER_H
#define MONSTER_H

#include <stdint.h>

typedef struct sprite_s Sprite;

typedef enum monster_type_e {
    MT_theUltimateMan,
    MT_slime
} MonsterType;

typedef enum monster_state_e {
    MS_idle,
    MS_move,
    MS_attackRanged,
    MS_attackMelee,
    MS_hurt,
    MS_dead
} MonsterState;

typedef struct monster_s {
    double x;
    double y;
    Sprite * sprite;
    int spriteFrameX;
    int spriteFrameY;
    MonsterType type;
    MonsterState state;
    uint8_t sawPlayer;
    double speed;
    uint8_t meleePerformAttack;
    double meleeSqrDistance;
    double meleeDamage;
    double meleeFrequency;
    double meleeNextAttack;
} Monster;

void monstersInit();
void monstersUpdate();
void monstersDraw();

Monster * monsterCreate(MonsterType type);

#endif
