#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include "config.h"

#define GFX_SCREEN_WIDTH 640
#define GFX_SCREEN_HEIGHT 360
#define GFX_TEXTURE_STORAGE_SIZE 256

#define GFX_WALLS_STARTINDEX 100

typedef struct texture_s {
    uint8_t * data;
    uint32_t width;
    uint32_t height;
} Texture;

typedef struct sprite_s {
    uint32_t textureId;
    uint32_t cellWidth;
    uint32_t cellHeight;
    uint32_t cellCountX;
    uint32_t cellCountY;
} Sprite;

void gfxInit(int argc, char * argv[]);
void gfxBegin();
void gfxEnd();
void gfxToggleFullscreen();
void gfxSetCamera(double x, double y, double angle, double fov);
void gfxSetRaycastingWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void gfxSetFontTexture(uint32_t id, uint32_t charWidth, uint32_t charHeight, uint32_t charCountX);
void gfxRenderWorld();
void gfxRenderSprite(Sprite * sprite, double x, double y, int frameX, int frameY);
void gfxRenderChar(char c, uint32_t x, uint32_t y);
void gfxRenderText(char * text, uint32_t x, uint32_t y);
void gfxRenderHud(Player * player);

int gfxLoadTexture(const char * filename, uint32_t id);

Sprite * createSpriteFromId(SpriteId id);

#endif
