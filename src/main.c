#include "engine.h"
#include "physfs.h"

int main(int argc, char* argv[])
{
    PHYSFS_init(argv[0]);
    PHYSFS_setSaneConfig("jancc", "raycaster", "pkg", 0, 0);
    PHYSFS_mount("./data/", "/", 0);
    gameInit(argc, argv);
    gameLoop();
    gameDestroy();
    PHYSFS_deinit();
    return 0;
}