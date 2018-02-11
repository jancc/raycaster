#include "engine.h"

Sprite testSprite;
double x;
double y;
double rot;

void gameInit() {
    x = 32;
    y = 32;
    rot = 0;
    testSprite.textureId = 1;
    worldCreateTilemap(512, 512);
    srand(56);
    worldRandomize();
    gfxInit();
    gfxSetRaycastingWindow(0, 0, GFX_SCREEN_WIDTH, GFX_SCREEN_HEIGHT);
    inputInit();
}

void movePlayer(double dx, double dy) {
    if(!worldGetCollisionInArea(x - 0.1 + dx, y - 0.1, 0.2, 0.2)) {
        x += dx;
    }
    if(!worldGetCollisionInArea(x - 0.1, y - 0.1 + dy, 0.2, 0.2)) {
        y += dy;
    }
}

void getInput() {
    double vx = 0;
    double vy = 0;
    double speed = 0.1;
    double turnspeed = 0.02;
    if(inputGetKey(INPUT_RUN)) {
        speed = 0.2;
        turnspeed = 0.05;
    }
    if(inputGetKey(INPUT_FORWARD)) {
        vx=speed;
    }
    if(inputGetKey(INPUT_BACKWARD)) {
        vx=-speed;
    }
    if(inputGetKey(INPUT_STRAFELEFT)) {
        vy=-speed;
    }
    if(inputGetKey(INPUT_STRAFERIGHT)) {
        vy=speed;
    }
    if(inputGetKey(INPUT_TURNLEFT)) {
        rot-=turnspeed;
    }
    if(inputGetKey(INPUT_TURNRIGHT)) {
        rot+=turnspeed;
    }
    vec2Rotate(&vx, &vy, rot);
    movePlayer(vx, vy);
}

void gameStep() {
    inputUpdate();
    if(inputGetFullscreen()) gfxToggleFullscreen();
    getInput();
    gfxBegin();
    gfxSetCamera(x, y, rot, 85);
    gfxRenderWorld();
    gfxRenderSprite(&testSprite, 31, 31);
    gfxEnd();
}

void gameLoop() {
    int is_running = 1;
    while(is_running) {
        gameStep();
        is_running = !inputGetQuit();
        SDL_Delay(10);
    }
}

void gameDestroy() {

}
