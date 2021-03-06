#include <SDL2/SDL_image.h>
#include "engine.h"
#include <physfs.h>

static SDL_Window* window;
static SDL_Renderer* renderer;
static int fullscreen;
static SDL_Texture* screen;
static SDL_Texture* raycasterScreen;
static SDL_Texture* textures[GFX_TEXTURE_STORAGE_SIZE];
static double* zbuffer;
static double cameraX, cameraY, cameraDirX, cameraDirY, cameraBaseplaneX,
    cameraBaseplaneY;
static uint32_t windowX, windowY, windowW, windowH;
static uint32_t fontTextureId, fontCharWidth, fontCharHeight, fontCharCountX;

void gfxInit(int argc, char* argv[])
{
    IMG_Init(IMG_INIT_PNG);
    window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, GFX_SCREEN_WIDTH,
                              GFX_SCREEN_HEIGHT, 0);
    SDL_RendererFlags rendererFlags = SDL_RENDERER_TARGETTEXTURE;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--force-sw") == 0) {
            rendererFlags |= SDL_RENDERER_SOFTWARE;
        } else if (strcmp(argv[i], "--force-hw") == 0) {
            rendererFlags |= SDL_RENDERER_ACCELERATED;
        }
    }
    renderer = SDL_CreateRenderer(window, -1, rendererFlags);
    SDL_SetWindowTitle(window, "This is not the game, it's just a tribute");
    SDL_ShowCursor(SDL_DISABLE);
    screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                               SDL_TEXTUREACCESS_TARGET, GFX_SCREEN_WIDTH,
                               GFX_SCREEN_HEIGHT);
    raycasterScreen = NULL;
    zbuffer = NULL;

    size_t texdefsCount = sizeof(texdefs) / sizeof(struct texdef_s);
    for (size_t i = 0; i < texdefsCount; i++) {
        struct texdef_s texdef = texdefs[i];
        gfxLoadTexture(texdef.filename, texdef.id);
    }

    size_t tiletexdefsCount = sizeof(tiletexdefs) / sizeof(struct tiletexdef_s);
    for (size_t i = 0; i < tiletexdefsCount; i++) {
        struct tiletexdef_s tiletexdef = tiletexdefs[i];
        gfxLoadTexture(tiletexdef.filename,
                       tiletexdef.id + GFX_WALLS_STARTINDEX);
    }

    gfxSetFontTexture(2, 8, 8, 16);
    gfxSetCamera(0, 0, 0, 90);
    fullscreen = 0;
}

void gfxBegin()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(renderer, screen);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void gfxEnd()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void gfxToggleFullscreen()
{
    fullscreen = !fullscreen;
    SDL_SetWindowFullscreen(window,
                            fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void gfxSetCamera(double x, double y, double angle, double fov)
{
    cameraX = x;
    cameraY = y;
    cameraDirX = 1;
    cameraDirY = 0;
    cameraBaseplaneX = 0;
    cameraBaseplaneY = fov / 90.0;
    vec2Rotate(&cameraDirX, &cameraDirY, angle);
    vec2Rotate(&cameraBaseplaneX, &cameraBaseplaneY, angle);
}

void gfxSetRaycastingWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    windowX = x;
    windowY = y;
    windowW = w;
    windowH = h;
    if (zbuffer != NULL) {
        free(zbuffer);
    }
    zbuffer = malloc(sizeof(double) * w);
    raycasterScreen =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                          SDL_TEXTUREACCESS_TARGET, windowW, windowH);
}

void gfxSetFontTexture(uint32_t id, uint32_t charWidth, uint32_t charHeight,
                       uint32_t charCountX)
{
    fontTextureId = id;
    fontCharWidth = charWidth;
    fontCharHeight = charHeight;
    fontCharCountX = charCountX;
}

void gfxRenderWorld()
{
    SDL_SetRenderTarget(renderer, raycasterScreen);
    SDL_Rect floorRect = {windowX, windowY + windowH / 2, windowW, windowH / 2};
    SDL_Rect ceilRect = {windowX, windowY, windowW, windowH / 2};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &floorRect);
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderFillRect(renderer, &ceilRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    double dirX = cameraDirX;
    double dirY = cameraDirY;
    double cameraplaneX = cameraBaseplaneX;
    double cameraplaneY = cameraBaseplaneY;

    for (uint32_t x = 0; x < windowW; x++) {
        double raycastX = 2 * x / (double)windowW - 1;
        double rayX = cameraX;
        double rayY = cameraY;
        double rayDirX = dirX + cameraplaneX * raycastX;
        double rayDirY = dirY + cameraplaneY * raycastX;

        int mapX = rayX;
        int mapY = rayY;
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;
        double sideDistX;
        double sideDistY;
        int stepX;
        int stepY;
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rayX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rayY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayY) * deltaDistY;
        }
        uint8_t hitdir = 0;
        uint8_t hitmask = 0;
        int side;
        while ((hitmask & hitdir) == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
                hitdir = stepX > 0 ? TILE_COLLIDER_EAST : TILE_COLLIDER_WEST;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
                hitdir = stepY > 0 ? TILE_COLLIDER_NORTH : TILE_COLLIDER_SOUTH;
            }
            hitmask = worldGetCollision(mapX, mapY);
        }
        if (side == 0) {
            perpWallDist = (mapX - rayX + (1 - stepX) / 2.0) / rayDirX;
        } else {
            perpWallDist = (mapY - rayY + (1 - stepY) / 2.0) / rayDirY;
        }
        double wallHitX;
        if (side == 0) {
            wallHitX = rayY + perpWallDist * rayDirY;
        } else {
            wallHitX = rayX + perpWallDist * rayDirX;
        }
        wallHitX -= floor(wallHitX);
        // TODO, if perpWallDist < 1 this is larger than windowH

        // World textures start at 100, and the first tileId to be textured is 1
        // So we add 99 to the tileId to get the offset
        int textureIndex = worldGetTile(mapX, mapY) + 99;
        int textureW, textureH;
        SDL_QueryTexture(textures[textureIndex], NULL, NULL, &textureW,
                         &textureH);
        SDL_Rect src;
        src.x = wallHitX * textureW;
        src.y = 0;
        src.w = 1;
        src.h = textureH;

        int lineHeight = (double)windowH / perpWallDist;
        /*
        if(lineHeight > windowH) {
            src.y += (lineHeight - windowH) / 16;
            src.h -= (lineHeight - windowH) / 8;
            lineHeight = windowH;
        }*/
        int y = windowH / 2 - lineHeight / 2;
        if (side == 0) {
            SDL_SetTextureColorMod(textures[textureIndex], 200, 200, 200);
        } else {
            SDL_SetTextureColorMod(textures[textureIndex], 250, 250, 250);
        }
        SDL_Rect dst;
        dst.x = x + windowX;
        dst.y = y + windowY;
        dst.w = 1;
        dst.h = lineHeight;
        SDL_RenderCopy(renderer, textures[textureIndex], &src, &dst);
        zbuffer[x] = perpWallDist;
    }
    /*
    Sprite rendering
    for(uint32_t i = 0; i < worldCountEnts(); i++) {
        Ent * ent = worldGetEnts()[i];
        if(ent->visible) {
            gfxRenderSprite(&ent->sprite, ent->x + 0.5, ent->y + 0.5);
        }
    }
    */
    SDL_SetRenderTarget(renderer, screen);
    SDL_Rect dst;
    dst.x = windowX;
    dst.y = windowY;
    dst.w = windowW;
    dst.h = windowH;
    SDL_RenderCopy(renderer, raycasterScreen, NULL, &dst);
}

void gfxRenderSprite(Sprite* sprite, double x, double y, int frameX, int frameY)
{
    double relativeX = cameraX - x;
    double relativeY = cameraY - y;
    double mul =
        1.0 / (cameraBaseplaneX * cameraDirY - cameraBaseplaneY * cameraDirX);
    // Multiply the relative coordinates with a "projection matrix"
    double cameraspaceX =
        relativeX * cameraDirY * mul + relativeY * -cameraDirX * mul;
    double cameraspaceY = relativeX * -cameraBaseplaneY * mul +
                          relativeY * cameraBaseplaneX * mul;
    // Since -0.0 < 0.0, I use -0.5 as a reference
    // This just makes Sprites that are behind or inside the player invisible
    if (cameraspaceY < -0.5) {
        double scale = fabs(windowH / cameraspaceY);
        SDL_Rect src;
        src.y = 64 * frameY;
        src.w = 1;
        src.h = 64;
        SDL_Rect dst;
        dst.x = (cameraspaceX / cameraspaceY + 1) * windowW / 2 - scale / 2;
        dst.y = windowH / 2 - scale / 2;
        dst.w = 1;
        dst.h = scale;
        for (int x = 0; x < (int)scale; x++) {
            src.x = x * (64.0 / scale) + 64 * frameX;
            if (dst.x >= (int32_t)windowW) {
                break;
            }
            if (dst.x >= 0 && zbuffer[dst.x] > fabs(cameraspaceY)) {
                SDL_RenderCopy(renderer, textures[sprite->textureId], &src,
                               &dst);
            }
            dst.x++;
        }
    }
}

void gfxRenderSprite2D(Sprite* sprite, uint32_t x, uint32_t y, uint32_t scale,
                       int frameX, int frameY)
{
    SDL_Rect src;
    src.x = sprite->cellWidth * frameX;
    src.y = sprite->cellHeight * frameY;
    src.w = sprite->cellWidth;
    src.h = sprite->cellHeight;
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = sprite->cellWidth * scale;
    dst.h = sprite->cellHeight * scale;
    SDL_RenderCopy(renderer, textures[sprite->textureId], &src, &dst);
}

void gfxRenderChar(char c, uint32_t x, uint32_t y)
{
    SDL_Rect src, dst;
    src.x = (c % fontCharCountX) * fontCharWidth;
    src.y = (c / fontCharCountX) * fontCharHeight;
    src.w = fontCharWidth;
    src.h = fontCharHeight;
    dst.x = x;
    dst.y = y;
    dst.w = fontCharWidth;
    dst.h = fontCharHeight;
    SDL_RenderCopy(renderer, textures[fontTextureId], &src, &dst);
}

void gfxRenderText(char* text, uint32_t x, uint32_t y)
{
    unsigned long length = strlen(text);
    uint32_t curX = x;
    uint32_t curY = y;
    for (unsigned long i = 0; i < length; i++) {
        if (text[i] == '\n') {
            curY += fontCharHeight;
            curX = x;
        } else {
            gfxRenderChar(text[i], curX, curY);
            curX += fontCharWidth;
        }
    }
}

int gfxLoadTexture(const char* filename, uint32_t id)
{
    PHYSFS_File* file = PHYSFS_openRead(filename);
    if (file == NULL) {
        printf("Failed to read image: %s\n", filename);
    }
    PHYSFS_sint64 filesize = PHYSFS_fileLength(file);
    void* buffer = malloc(filesize * sizeof(char));
    PHYSFS_readBytes(file, buffer, filesize);
    SDL_RWops* fileRWops = SDL_RWFromMem(buffer, filesize);
    SDL_Surface* tempSurf = IMG_Load_RW(fileRWops, 0);
    SDL_RWclose(fileRWops);
    free(buffer);
    PHYSFS_close(file);
    if (tempSurf == NULL) {
        return 0;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurf);
    textures[id] = texture;
    SDL_FreeSurface(tempSurf);
    return 1;
}

void gfxRenderHud(Player* player)
{
    char buf[255];
    snprintf(buf, 255, "Health: %d\nWeapon: %s\nAmmo: %d\nScore: %d",
             player->health, "AK-47", 10000, 404);
    gfxRenderText(buf, 16, GFX_SCREEN_HEIGHT - 48);

    // Cycle horizontally through all sprite cells
    // This animation should take exactly as long as the weapon frequency is
    // However the first cell is skipped, as it implies the weapon is in it's
    // idle state
    int weaponAnimFrame = 0;
    double animTime = getTime() - player->weapon.animationStartTime;
    if (player->weapon.animationStartTime + player->weapon.frequency >
        getTime()) {
        int frames = player->weapon.sprite->cellCountX - 1;
        weaponAnimFrame =
            (int)((animTime * frames) / player->weapon.frequency) % frames + 1;
    }
    gfxRenderSprite2D(player->weapon.sprite, windowX + windowW - 256,
                      windowY + windowH - 256, 4, weaponAnimFrame, 0);
}

Sprite* createSpriteFromId(SpriteId id)
{
    Sprite* sprite = malloc(sizeof(Sprite));
    memset(sprite, 0, sizeof(Sprite));
    size_t spritedefsCount = sizeof(spritedefs) / sizeof(struct spritedef_s);
    for (size_t i = 0; i < spritedefsCount; i++) {
        struct spritedef_s spritedef = spritedefs[i];
        if (spritedef.id == id) {
            sprite->textureId = spritedef.texture;
            sprite->cellWidth = spritedef.cellWidth;
            sprite->cellHeight = spritedef.cellHeight;
            sprite->cellCountX = spritedef.cellCountX;
            sprite->cellCountY = spritedef.cellCountY;
        }
    }
    return sprite;
}
