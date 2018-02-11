#include "engine.h"

int main(int argc, char * argv[]) {
    gameInit();
    gameLoop();
    gameDestroy();
    return 0;
}