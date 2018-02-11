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
    worldRandomize();
    gfxInit();
    gfxSetRaycastingWindow(0, 0, GFX_SCREEN_WIDTH, GFX_SCREEN_HEIGHT);
    inputInit();
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
    x += vx;
    y += vy;
}

void gameStep() {
    inputUpdate();
    if(inputGetFullscreen()) gfxToggleFullscreen();
    getInput();
    gfxBegin();
    gfxSetCamera(x, y, rot, 85);
    gfxRenderWorld();
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
