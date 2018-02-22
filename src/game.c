#include "engine.h"

Sprite testSprite;
double x;
double y;
double rot;
uint8_t showMenu;
uint8_t forceQuit;

void gameInit() {
    x = 32;
    y = 32;
    rot = 0;
    showMenu = 0;
    forceQuit = 0;
    testSprite.textureId = 1;
    worldCreateTilemap(512, 512);
    srand(56);
    worldRandomize();
    gfxInit();
    gfxSetRaycastingWindow(0, 0, GFX_SCREEN_WIDTH, GFX_SCREEN_HEIGHT - 64);
    inputInit();
    menuInit();
    worldInit();
    worldSpawnEnt(31.5, 31.5);
    worldSpawnEnt(32.5, 31.5);
    worldSpawnEnt(33.5, 31.5);
    worldSpawnEnt(34.5, 31.5);
    worldSpawnEnt(35.5, 31.5);
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
    double speed = 0.05;
    double turnspeed = 0.03;
    if(inputGetKey(INPUT_RUN)) {
        speed = 0.1;
        turnspeed = 0.05;
    }
    if(inputGetKey(INPUT_FORWARD)) {
        vx=1;
    }
    if(inputGetKey(INPUT_BACKWARD)) {
        vx=-1;
    }
    if(inputGetKey(INPUT_STRAFELEFT)) {
        vy=-1;
    }
    if(inputGetKey(INPUT_STRAFERIGHT)) {
        vy=1;
    }
    if(inputGetKey(INPUT_TURNLEFT)) {
        rot-=turnspeed;
    }
    if(inputGetKey(INPUT_TURNRIGHT)) {
        rot+=turnspeed;
    }
    vec2Rotate(&vx, &vy, rot);
    vec2Normalize(&vx, &vy);
    movePlayer(vx * speed, vy * speed);
}

void doMenuIngame() {
    if(menuDoButton("continue")) showMenu = 0;
    if(menuDoButton("toggle fullscreen")) gfxToggleFullscreen();
    if(menuDoButton("fill world with bullshit")) worldRandomize();
    if(menuDoButton("quit")) forceQuit = 1;
}

void drawEnts() {
    Ent ** ents = worldGetEnts();
    double dx = 1;
    double dy = 0;
    vec2Rotate(&dx, &dy, rot);
    Ray ray = {x, y, dx, dy};
    AABB box;
    for(size_t i = 0; i < worldGetEntsSize(); i++) {
        if(ents[i] != NULL) {
            box.x1 = ents[i]->x - 0.5;
            box.x2 = ents[i]->x + 0.5;
            box.y1 = ents[i]->y - 0.5;
            box.y2 = ents[i]->y + 0.5;
            if(!worldHitscanTest(&ray, &box)) gfxRenderSprite(&testSprite, ents[i]->x, ents[i]->y);
        }
    }
}

void gameStep() {
    inputUpdate();
    if(inputGetFullscreen()) gfxToggleFullscreen();
    if(inputGetKeyDown(INPUT_TOGGLEMENU)) showMenu = !showMenu;
    if(!showMenu) {
        getInput();
    }
    gfxBegin();
    gfxSetCamera(x, y, rot, 90);
    gfxRenderWorld();
    drawEnts();
    if(showMenu) {
        menuBegin();
        doMenuIngame();
        menuEnd();
    }
    gfxEnd();
}

void gameLoop() {
    uint8_t isRunning = 1;
    while(isRunning) {
        gameStep();
        isRunning = !forceQuit && !inputGetQuit();
        SDL_Delay(10);
    }
}

void gameDestroy() {

}
