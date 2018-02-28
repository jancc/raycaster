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
    double dx = 1;
    double dy = 0;
    vec2Rotate(&dx, &dy, player->rot);
    Ray ray = {player->x, player->y, dx, dy};
    HitscanOut hit;
    if(worldHitscan(&ray, &hit)) {
        worldDespawnMonster(hit.monster, 1);
    }
}

void playerUpdate(Player * player) {
    playerDoMovement(player);
    playerDoShooting(player);
}