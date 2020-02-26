#include "engine.h"

void monstersInit() {}

// TODO: turn into an actual state machine thingy
void updateMonster(Monster* monster)
{
    Player* player = worldGetPlayer();
    double sqrPlayerDist =
        vec2SqrDist(monster->x, monster->y, player->x, player->y);
    double dx = player->x - monster->x;
    double dy = player->y - monster->y;
    vec2Normalize(&dx, &dy);
    if (monster->sawPlayer) {
        if (sqrPlayerDist > monster->meleeSqrDistance) {
            monster->state = MS_move;
            monster->x += dx * monster->speed;
            monster->y += dy * monster->speed;
        } else if (monster->meleeNextAttack < getTime()) {
            monster->state = MS_attackMelee;
            player->health -= monster->meleeDamage;
            monster->meleeNextAttack = getTime() + monster->meleeFrequency;
        }
    } else {
        monster->state = MS_idle;
        Ray ray = {monster->x, monster->y, dx, dy};
        HitscanOut out;
        worldHitscan(&ray, &out, 1, 0);
        monster->sawPlayer =
            out.hit &&
            vec2SqrDist(monster->x, monster->y, out.x, out.y) > sqrPlayerDist;
    }

    // TODO: better, more versitile animation code
    switch (monster->state) {
    case MS_idle:
        monster->spriteFrameX = 0;
        monster->spriteFrameY = 0;
        break;
    case MS_move:
        monster->spriteFrameX = (int)(getTime() * 4) % 4;
        monster->spriteFrameY = 0;
        break;
    case MS_attackMelee:
        monster->spriteFrameX = (int)(getTime() * 6) % 4;
        monster->spriteFrameY = 1;
        break;
    }
}

void monstersUpdate()
{
    Monster** monsters = worldGetMonsters();
    size_t monstersSize = worldGetMonstersSize();
    for (size_t i = 0; i < monstersSize; i++) {
        Monster* monster = monsters[i];
        if (monster == NULL) continue;
        updateMonster(monster);
    }
}

void monstersDraw()
{
    Monster** monsters = worldGetMonsters();
    for (size_t i = 0; i < worldGetMonstersSize(); i++) {
        Monster* monster = monsters[i];
        if (monster != NULL) {
            gfxRenderSprite(monster->sprite, monster->x, monster->y,
                            monster->spriteFrameX, monster->spriteFrameY);
        }
    }
}

Monster* monsterCreate(MonsterType type)
{
    Monster* monster = malloc(sizeof(Monster));
    memset(monster, 0, sizeof(Monster));
    size_t monsterdefCount = sizeof(monsterdefs) / sizeof(struct monsterdef_s);
    for (size_t i = 0; i < monsterdefCount; i++) {
        struct monsterdef_s monsterdef = monsterdefs[i];
        if (monsterdef.type == type) {
            monster->speed = monsterdef.speed;
            monster->meleeSqrDistance =
                monsterdef.meleeAttackDistance * monsterdef.meleeAttackDistance;
            monster->meleeDamage = monsterdef.meleeDamage;
            monster->meleeFrequency = monsterdef.meleeFrequency;
            monster->sprite = createSpriteFromId(monsterdef.spriteId);
        }
    }
    return monster;
}

void monsterFree(Monster* monster)
{
    free(monster->sprite);
    free(monster);
}