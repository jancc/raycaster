#include "engine.h"

static double initTime;
static double time;

void timingInit()
{
    SDL_InitSubSystem(SDL_INIT_TIMER);
    initTime = SDL_GetTicks() / 1000.0;
}

void timingUpdate()
{
    double now = SDL_GetTicks() / 1000.0;
    time = now - initTime;
}

double getTime() { return time; }