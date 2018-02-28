#include "engine.h"

Sprite testSprite;
uint8_t showMenu;
uint8_t forceQuit;

void gameInit() {
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
    worldResetPlayer(32, 32);
    worldSpawnMonster(31.5, 31.5);
    worldSpawnMonster(32.5, 31.5);
    worldSpawnMonster(33.5, 31.5);
    worldSpawnMonster(34.5, 31.5);
    worldSpawnMonster(35.5, 31.5);
}

void doMenuIngame() {
    if(menuDoButton("continue")) showMenu = 0;
    if(menuDoButton("toggle fullscreen")) gfxToggleFullscreen();
    if(menuDoButton("fill world with bullshit")) worldRandomize();
    if(menuDoButton("quit")) forceQuit = 1;
}

void drawMonsters() {
    Monster ** monsters = worldGetMonsters();
    for(size_t i = 0; i < worldGetMonstersSize(); i++) {
        if(monsters[i] != NULL) {
            gfxRenderSprite(&testSprite, monsters[i]->x, monsters[i]->y);
        }
    }
}

void gameStep() {
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
    drawMonsters();
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
