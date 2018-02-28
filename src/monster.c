#include "engine.h"

void monstersUpdate() {
    Monster ** monsters = worldGetMonsters();
    size_t monstersSize = worldGetMonstersSize();
    for(size_t i = 0; i < monstersSize; i++) {
        Monster * monster = monsters[i];
        if(monster == NULL) continue;
        monster->x += (rand() % 4 - 2) * 0.01;
        monster->y += (rand() % 4 - 2) * 0.01;
    }
}