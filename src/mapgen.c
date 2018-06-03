#include "engine.h"

typedef enum direction_e {
    DIR_NORTH,
    DIR_SOUTH,
    DIR_WEST,
    DIR_EAST
} Direction;

typedef struct mapgenSpawner {
    int active;
    uint32_t x;
    uint32_t y;
    uint32_t lifetime;
    uint32_t generation;
    Direction dir;
} MapgenSpawner;

static MapgenSpawner * spawners = NULL;
static int spawnersMax = 8;
static int activeSpawners = 0;
static int generationStep = 0;
static uint32_t rngSeed = 1234;
static uint32_t rngA = 1103515245;
static uint32_t rngC = 12345;
static uint32_t rngM = 4294967295; // 2^32

static uint32_t mapWidth;
static uint32_t mapHeight;
static uint32_t playerX;
static uint32_t playerY;

// RNG used for this mapgen, to allow for consistency across platforms
uint32_t mapgenRand() {
    rngSeed = (rngA * rngSeed + rngC) % rngM;
    return rngSeed;
}

// Empties an area
void spawnRoom(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) {
    x1 = clamp(x1, 1, mapWidth - 1);
    x2 = clamp(x2, 1, mapWidth - 1);
    y1 = clamp(y1, 1, mapHeight - 1);
    y2 = clamp(y2, 1, mapHeight - 1);
    for(uint32_t y = y1; y < y2; y++) {
        for(uint32_t x = x1; x < x2; x++) {
            worldSetTile(x, y, 0);
        }
    }
}

// Gets first index of inactive spawner or -1 if no one is free
int getFreeSpawnerIndex() {
    for(int i = 0; i < spawnersMax; i++) {
        if(!spawners[i].active) return i;
    }
    return -1;
}

// Creates a new spawner at give position
void spawnerCreate(uint32_t x, uint32_t y, uint32_t lifetime) {
    int index = getFreeSpawnerIndex();
    printf("%i\n", index);
    if(index == -1) return;
    activeSpawners++;
    MapgenSpawner * spawner = &spawners[index];
    spawner->active = 1;
    spawner->x = x;
    spawner->y = y;
    spawner->lifetime = lifetime;
    spawner->generation = 0;
    int dir = mapgenRand() % 4;
    switch(dir) {
    case 0:
        spawner->dir = DIR_NORTH;
        break;
    case 1:
        spawner->dir = DIR_SOUTH;
        break;
    case 2:
        spawner->dir = DIR_WEST;
        break;
    case 3:
        spawner->dir = DIR_EAST;
        break;
    }
}

void spawnerStep(MapgenSpawner * spawner) {
    // Calculate wether the spawner should change the direction it's moving to
    int changeDirection = mapgenRand() % 100 > 95;
    if(changeDirection) {
        int dir = mapgenRand() % 4;
        switch(dir) {
        case 0:
            spawner->dir = DIR_NORTH;
            break;
        case 1:
            spawner->dir = DIR_SOUTH;
            break;
        case 2:
            spawner->dir = DIR_WEST;
            break;
        case 3:
            spawner->dir = DIR_EAST;
            break;
        }
    }

    // Calculate movement
    int dx = 0;
    int dy = 0;
    switch(spawner->dir) {
    case DIR_NORTH:
        dy = -1;
        break;
    case DIR_SOUTH:
        dy = 1;
        break;
    case DIR_EAST:
        dx = 1;
        break;
    case DIR_WEST:
        dx = -1;
        break;
    }

    uint32_t newX = spawner->x + dx;
    uint32_t newY = spawner->y + dy;
    
    // Only move if this does NOT make the spawner move out of map boundaries
    // A gap of 1 tile is left, so the player can't stare into the segfault area beyond
    if(newX < mapWidth - 1) {
        spawner->x = newX;
    }
    if(newY < mapHeight - 1) {
        spawner->y = newY;
    }

    // Clear tile under spawner
    worldSetTile(spawner->x, spawner->y, 0);

    // Check if spawner should spawn another spawner
    int spawnChild = mapgenRand() % 100 > 95;
    if(spawnChild) spawnerCreate(spawner->x, spawner->y, mapgenRand() % 16 + 16);

    // Check if spawner should create a room here
    int doSpawnRoom = mapgenRand() % 100 > 66;
    if(doSpawnRoom) {
        int x1 = spawner->x - mapgenRand() % 4 + 2;
        int x2 = spawner->x + mapgenRand() % 4 + 2;
        int y1 = spawner->y - mapgenRand() % 4 + 2;
        int y2 = spawner->y + mapgenRand() % 4 + 2;
        spawnRoom(x1, y1, x2, y2);
    }

    // Once a spawners lifetime is up, it becomes inactive
    spawner->lifetime--;
    if(spawner->lifetime == 0) {
        spawner->active = 0;
        activeSpawners--;
    }
}

void mapgenGenerate(uint32_t width, uint32_t height, uint32_t seed) {
    rngSeed = seed;
    mapWidth = width;
    mapHeight = height;
    generationStep = 0;

    // Init spawner array
    spawners = calloc(sizeof(MapgenSpawner), spawnersMax);

    // Fill tilemap with walls
    worldCreateTilemap(width, height);
    uint32_t tiletypes = sizeof(tiletexdefs) / sizeof(struct tiletexdef_s);
    for(uint32_t y = 0; y < height; y++) {
        for(uint32_t x = 0; x < width; x++) {
            worldSetTile(x, y, 1 + mapgenRand() % tiletypes);
        }
    }

    uint32_t startX = mapgenRand() % (width - 2) + 1;
    uint32_t startY = mapgenRand() % (height - 2) + 1;
    worldSetTile(startX, startY, 0);
    playerX = startX;
    playerY = startY;
    spawnerCreate(startX, startY, 100);

    // Loop through active spawners until all are inactive or we did enough steps
    while(activeSpawners > 0 && generationStep < 100) {
        for(int i = 0; i < spawnersMax; i++) {
            if(spawners[i].active) {
                spawnerStep(&spawners[i]);
            }
        }
        generationStep++;
    }

    // free spawner array
    free(spawners);
}

uint32_t mapgenGetPlayerX() {
    return playerX;
}

uint32_t mapgenGetPlayerY() {
    return playerY;
}
