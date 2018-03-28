#include "engine.h"

static Sprite testSprite;
static uint8_t showMenu;
static uint8_t forceQuit;

void gameInit() {
    timingInit();
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
    monstersInit();
    worldResetPlayer(32, 32);
    worldSpawnMonster(monsterCreate(MT_slime), 31.5, 31.5);
    worldSpawnMonster(monsterCreate(MT_slime), 33.5, 32.5);
    worldSpawnMonster(monsterCreate(MT_slime), 31.5, 34.5);
    worldSpawnMonster(monsterCreate(MT_slime), 33.5, 33.5);
}

void doMenuIngame() {
    if(menuDoButton("continue")) showMenu = 0;
    if(menuDoButton("toggle fullscreen")) gfxToggleFullscreen();
    if(menuDoButton("fill world with bullshit")) worldRandomize();
    if(menuDoButton("quit")) forceQuit = 1;
}

void gameStep() {
    timingUpdate();
    inputUpdate();
    if(inputGetFullscreen()) gfxToggleFullscreen();
    if(inputGetKeyDown(INPUT_TOGGLEMENU)) showMenu = !showMenu;
    Player * player = worldGetPlayer();
    if(!showMenu) {
        playerUpdate(player);
        monstersUpdate();
    }
    gfxBegin();
    gfxSetCamera(player->x, player->y, player->rot, 90);
    gfxRenderWorld();
    monstersDraw();
    gfxRenderHud(player);
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
