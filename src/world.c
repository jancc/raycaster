#include "engine.h"

static uint8_t * tilemap;
static uint32_t tilemapWidth;
static uint32_t tilemapHeight;
static Player * player;
static Monster ** monsters;
static size_t monstersArraySize;
static size_t monstersCount;

// double the size of our monsters array
void monstersArrayExpand() {
    monsters = realloc(monsters, monstersArraySize * 2 * sizeof(Monster *));
    for(size_t i = monstersArraySize; i < monstersArraySize * 2; i++) {
        monsters[i] = NULL;
    }
    monstersArraySize *= 2;
}

void worldInit() {
    player = malloc(sizeof(Player));
    worldResetPlayer(1, 1);
    monstersArraySize = 1;
    monstersCount = 0;
    monsters = malloc(monstersArraySize * sizeof(Monster *));
    monsters[0] = NULL;
}

void worldCreateTilemap(uint32_t width, uint32_t height) {
    tilemap = calloc(width * height, sizeof(uint8_t));
    tilemapWidth = width;
    tilemapHeight = height;
}

void worldRandomize() {
    for(uint32_t y = 0; y < tilemapHeight; y++) {
        for(uint32_t x = 0; x < tilemapWidth; x++) {
            if(y == 0 || y == tilemapHeight - 1 || x == 0 || x == tilemapWidth - 1) {
                worldSetTile(x, y, 1);
            } else if (rand() % 100 > 95) {
                worldSetTile(x, y, 1 + rand() % 7);
            }
        }
    }
}

void worldSetTile(uint32_t x, uint32_t y, uint8_t value) {
    tilemap[y * tilemapHeight + x] = value;
}

uint8_t worldGetTile(uint32_t x, uint32_t y) {
    return tilemap[y * tilemapHeight + x];
}

uint8_t worldGetCollision(uint32_t x, uint32_t y) {
    return worldGetTile(x, y) != 0;
}

uint8_t worldGetCollisionInArea(double areaX, double areaY, double areaW, double areaH) {
    uint32_t xStart = (uint32_t)areaX;
    uint32_t yStart = (uint32_t)areaY;
    uint32_t xEnd = (uint32_t)(areaX + areaW);
    uint32_t yEnd = (uint32_t)(areaY + areaH);
    for(uint32_t y = yStart; y <= yEnd; y++) {
        for(uint32_t x = xStart; x <= xEnd; x++) {
            if(worldGetCollision(x, y)) return 1;
        }
    }
    return 0;
}

void worldResetPlayer(double x, double y) {
    memset(player, 0, sizeof(Player));
    player->x = x;
    player->y = y;
    player->health = 100;
}

Player * worldGetPlayer() {
    return player;
}

Monster * worldSpawnMonster(Monster * monster, double x, double y) {
    monster->x = x;
    monster->y = y;
    if(monstersCount + 1 > monstersArraySize) {
        monstersArrayExpand();
    }
    for(size_t i = 0; i < monstersArraySize; i++) {
        if(monsters[i] == NULL) {
            monsters[i] = monster;
            monstersCount++;
            return monster;
        }
    }
    // should not happen normally
    free(monster);
    return NULL;
}

void worldDespawnMonster(Monster * monster, uint8_t autoFree) {
    for(size_t i = 0; i < monstersArraySize; i++) {
        if(monsters[i] == monster) {
            monsters[i] = NULL;
        }
    }
    if(autoFree) free(monster);
}

Monster ** worldGetMonsters() {
    return monsters;
}

size_t worldGetMonstersSize() {
    return monstersArraySize;
}

/*
 * Adapted from:
 * https://tavianator.com/fast-branchless-raybounding-box-intersections/
*/
uint8_t testRayBoxIntersection(Ray * ray, AABB * box) {
    double invDX = 1 / ray->dx;
    double invDY = 1 / ray->dy;
    double tx1 = (box->x1 - ray->x)*invDX;
    double tx2 = (box->x2 - ray->x)*invDX;

    double tmin = fmin(tx1, tx2);
    double tmax = fmax(tx1, tx2);

    double ty1 = (box->y1 - ray->y)*invDY;
    double ty2 = (box->y2 - ray->y)*invDY;

    tmin = fmax(tmin, fmin(ty1, ty2));
    tmax = fmin(tmax, fmax(ty1, ty2));

    return tmax >= tmin && tmax >= 0;
}

uint8_t worldHitscanMonsters(Ray * ray, Monster ** monster) {
    *monster = NULL;
    uint8_t hit = 0;
    double sqrDistanceTest = INFINITY;
    AABB aabb;
    for(size_t i = 0; i < monstersArraySize; i++) {
        if(monsters[i] != NULL) {
            aabb.x1 = monsters[i]->x - 0.25;
            aabb.x2 = monsters[i]->x + 0.25;
            aabb.y1 = monsters[i]->y - 0.25;
            aabb.y2 = monsters[i]->y + 0.25;
            if(testRayBoxIntersection(ray, &aabb)) {
                // only replace monster if the distance to it would be larger
                double sqrDistance = vec2SqrDist(ray->x, ray->y, monsters[i]->x, monsters[i]->y);
                if(sqrDistance < sqrDistanceTest) {
                    sqrDistanceTest = sqrDistance;
                }
                *monster = monsters[i];
                hit = 1;
            }
        }
    }
    return hit;
}

uint8_t worldHitscanTiles(Ray * ray, double * x, double * y) {
    int mapX = ray->x;
    int mapY = ray->y;
    double deltaDistX = sqrt(1 + (ray->dy*ray->dy) / (ray->dx*ray->dx));
    double deltaDistY = sqrt(1 + (ray->dx*ray->dx) / (ray->dy*ray->dy));
    double sideDistX;
    double sideDistY;
    int stepX;
    int stepY;
    if(ray->dx < 0) {
        stepX = -1;
        sideDistX = (ray->x - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - ray->x) * deltaDistX;
    }
    if(ray->dy < 0) {
        stepY = -1;
        sideDistY = (ray->y - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - ray->y) * deltaDistY;
    }
    int hit = 0;
    while(hit == 0) {
        if(sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
        }
        if(worldGetTile(mapX, mapY) > 0) {
            hit = 1;
        }
    }
    *x = mapX;
    *y = mapY;
    return hit;
}

uint8_t worldHitscan(Ray * ray, HitscanOut * out, uint8_t tiles, uint8_t monsters) {
    memset(out, 0, sizeof(HitscanOut));
    double x = INFINITY;
    double y = INFINITY;
    if(tiles) {
        out->hit = worldHitscanTiles(ray, &x, &y);
    }
    Monster * monster = NULL;
    if(monsters) {
        out->hit = worldHitscanMonsters(ray, &monster);
    }
    if(monster != NULL) {
        double sqrMonsterDist = (monster->x-ray->x)*(monster->x-ray->x)+(monster->y-ray->y)*(monster->y-ray->y);
        double sqrTileDist = (x-ray->x)*(x-ray->x)+(y-ray->y)*(y-ray->y);
        if(sqrMonsterDist < sqrTileDist) {
            // monster closer
            out->monster = monster;
            out->x = monster->x;
            out->y = monster->y;
        }
    } else {
        // tile closer
        out->x = x;
        out->y = y;
    }
    return out->hit;
}

void worldClear() {
    free(tilemap);
    tilemapWidth = 0;
    tilemapHeight = 0;
}
