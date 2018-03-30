#include "engine.h"

void playerDoMovement(Player * player) {
    double vx = 0;
    double vy = 0;
    double speed = 0.05;
    double turnspeed = 0.03;

    if(inputGetKey(INPUT_RUN)) {
        speed = 0.1;
        turnspeed = 0.05;
    }
    if(inputGetKey(INPUT_FORWARD)) vx=1;
    if(inputGetKey(INPUT_BACKWARD)) vx=-1;
    if(inputGetKey(INPUT_STRAFELEFT)) vy=-1;
    if(inputGetKey(INPUT_STRAFERIGHT)) vy=1;
    if(inputGetKey(INPUT_TURNLEFT)) player->rot-=turnspeed;
    if(inputGetKey(INPUT_TURNRIGHT)) player->rot+=turnspeed;

    vec2Rotate(&vx, &vy, player->rot);
    vec2Normalize(&vx, &vy);
    vx *= speed;
    vy *= speed;
    if(!worldGetCollisionInArea(player->x - 0.1 + vx, player->y - 0.1, 0.2, 0.2)) {
        player->x += vx;
    }
    if(!worldGetCollisionInArea(player->x - 0.1, player->y - 0.1 + vy, 0.2, 0.2)) {
        player->y += vy;
    }
}

void playerDoShooting(Player * player) {
    if(!inputGetKeyDown(INPUT_FIRE)) return;
    if(player->weapon.nextShot > getTime()) return;
    player->weapon.animationStartTime = getTime();
    player->weapon.nextShot = getTime() + player->weapon.frequency;
    double dx = 1;
    double dy = 0;
    vec2Rotate(&dx, &dy, player->rot);
    Ray ray = {player->x, player->y, dx, dy};
    HitscanOut hit;
    worldHitscan(&ray, &hit, 1, 1);
    double hitSqrDist = vec2SqrDist(hit.x, hit.y, player->x, player->y);
    if(hit.hit && hit.monster != NULL && hitSqrDist < player->weapon.maxSqrDistance) {
        worldDespawnMonster(hit.monster, player->weapon.damage);
    }
}

void playerSelectWeapon(Player * player, WeaponType type) {
    if(player->weapon.sprite != NULL) {
        free(player->weapon.sprite);
    }
    size_t weapondefsCount = sizeof(weapondefs) / sizeof(struct weapondef_s);
    for(size_t i = 0; i < weapondefsCount; i++) {
        struct weapondef_s weapondef = weapondefs[i];
        if(weapondef.weaponType == type) {
            player->weapon.damage = weapondef.damage;
            player->weapon.frequency = weapondef.frequency;
            player->weapon.maxSqrDistance = weapondef.maxDistance * weapondef.maxDistance;
            player->weapon.sprite = createSpriteFromId(weapondef.spriteId);
            player->weapon.animationStartTime = 0;
        }
    }
}

void playerUpdate(Player * player) {
    playerDoMovement(player);
    playerDoShooting(player);
}