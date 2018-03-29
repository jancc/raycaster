#ifndef CONFIG_H
#define CONFIG_H

typedef enum textures_e {
    TEX_FONT_POTASH = 2,
    TEX_ULTIMATEMAN = 1,
    TEX_SLIME = 3,
    TEX_VIEW_KNIFE = 4
} TextureId;
static struct texdef_s {int id; const char * filename;} const texdefs[] = {
    {0, "gfx/sheep.png"},
    {TEX_ULTIMATEMAN, "gfx/theultimateman.png"},
    {TEX_FONT_POTASH, "gfx/ui/potash_8x8.png"},
    {TEX_SLIME, "gfx/enemies/slime.png"},
    {TEX_VIEW_KNIFE, "gfx/weapons/view_knive.png"}
};

static struct tiletexdef_s {int id; const char * filename;} const tiletexdefs[] = {
    {0, "gfx/walls/brickwall01.png"},
    {1, "gfx/walls/brickwall02.png"},
    {2, "gfx/walls/brickwall03.png"},
    {3, "gfx/walls/brickwall04.png"},
    {4, "gfx/walls/brickwall05.png"},
    {5, "gfx/walls/dirtwall01.png"},
    {6, "gfx/walls/woodwall01.png"}
};

typedef enum sprites_e {
    SPR_ULTIMATEMAN = 0,
    SPR_SLIME = 1,
    SPR_VIEW_KNIFE = 2
} SpriteId;
static struct spritedef_s {
    TextureId texture;
    uint32_t id, cellWidth, cellHeight, cellCountX, cellCountY;
} const spritedefs[] = {
    {TEX_ULTIMATEMAN, SPR_ULTIMATEMAN, 64, 64, 1, 1},
    {TEX_SLIME, SPR_SLIME, 64, 64, 4, 4},
    {TEX_VIEW_KNIFE, SPR_VIEW_KNIFE, 64, 64, 4, 1}
};

typedef enum weapons_e {
    WEP_KNIFE = 0
} WeaponType;
static struct weapondef_s {
    WeaponType weaponType;
    SpriteId spriteId;
    double maxDistance, damage, frequency;
} const weapondefs[] = {
    {WEP_KNIFE, SPR_VIEW_KNIFE, 1, 1, 1}
};

typedef enum monster_type_e {
    MT_theUltimateMan,
    MT_slime
} MonsterType;
static struct monsterdef_s {
    MonsterType type;
    SpriteId spriteId;
    double speed, meleeAttackDistance, meleeDamage, meleeFrequency;
} const monsterdefs[] = {
    {MT_theUltimateMan, SPR_ULTIMATEMAN, 0.01, 1, 1, 1},
    {MT_slime, SPR_SLIME, 0.01, 1, 1, 1}
};

#endif