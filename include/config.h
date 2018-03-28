#ifndef CONFIG_H
#define CONFIG_H

#include "gfx.h"
#include "monster.h"

typedef enum textures_e {
    TEX_FONT_POTASH = 2,
    TEX_ULTIMATEMAN = 1,
    TEX_SLIME = 3,
    TEX_VIEW_KNIFE = 4
} TextureId;
static struct texdef_s {int id; const char * filename;} texdefs[] = {
    {0, "data/gfx/sheep.png"},
    {TEX_ULTIMATEMAN, "data/gfx/theultimateman"},
    {TEX_FONT_POTASH, "data/gfx/ui/potash_8x8.png"},
    {TEX_SLIME, "data/gfx/enemies/slime.png"},
    {TEX_VIEW_KNIFE, "data/gfx/weapons/view_knive.png"}
};

static struct tiletexdef_s {int id; const char * filename;} tiletexdefs[] = {
    {0, "data/gfx/walls/brickwall01.png"},
    {1, "data/gfx/walls/brickwall02.png"},
    {2, "data/gfx/walls/brickwall03.png"},
    {3, "data/gfx/walls/brickwall04.png"},
    {4, "data/gfx/walls/brickwall05.png"},
    {5, "data/gfx/walls/dirtwall01.png"},
    {6, "data/gfx/walls/woodwall01.png"}
};

typedef enum sprites_e {
    SPR_ULTIMATEMAN = 0,
    SPR_SLIME = 1,
    SPR_VIEW_KNIFE = 2
} SpriteId;
static struct spritedef_s {
    TextureId texture;
    uint32_t id, cellWidth, cellHeight, cellCountX, cellCountY;
} spritedefs[] = {
    {TEX_ULTIMATEMAN, SPR_ULTIMATEMAN, 64, 64, 1, 1},
    {TEX_SLIME, SPR_SLIME, 64, 64, 4, 4},
    {TEX_VIEW_KNIFE, SPR_VIEW_KNIFE, 64, 64, 4, 1}
};

const uint32_t weaponsCount = 1;
typedef enum weapons_e {
    WEP_KNIFE = 0
} WeaponType;
static struct weapondef_s {
    WeaponType weaponType;
    SpriteId spriteId;
    double maxDistance, damage, frequency;
} weapondefs[weaponsCount] = {
    {WEP_KNIFE, SPR_VIEW_KNIFE, 1, 1, 1}
};

static struct monsterdef_s {
    MonsterType type;
    uint32_t spriteId;
    double speed, meleeAttackDistance, meleeDamage, meleeFrequency;
} monsterdefs[] = {
    {MT_theUltimateMan, SPR_ULTIMATEMAN, 0.01, 1, 1, 1},
    {MT_slime, SPR_SLIME, 0.01, 1, 1, 1}
};

#endif