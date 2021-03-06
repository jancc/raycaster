#include "engine.h"

#define SDL_MAX_SCANCODES 284

static SDL_Event event;
static int quit;
static int fullscreen;
static uint8_t keys[INPUT_MAXKEYS];
static uint8_t keysDown[INPUT_MAXKEYS];
static uint8_t sdlScancodeLookup[SDL_MAX_SCANCODES];

void resetEvents()
{
    fullscreen = 0;
    quit = 0;
}

void resetKeysDown()
{
    memset(&keysDown, 0, sizeof(keysDown) * sizeof(uint8_t));
}

void resetKeys() { memset(&keys, 0, sizeof(keys) * sizeof(uint8_t)); }

void inputInit()
{
    resetKeys();
    resetEvents();
    for (uint32_t i = 0; i < SDL_MAX_SCANCODES; i++) {
        sdlScancodeLookup[i] = INPUT_UNDEF;
    }
    sdlScancodeLookup[SDL_SCANCODE_W] = INPUT_FORWARD;
    sdlScancodeLookup[SDL_SCANCODE_S] = INPUT_BACKWARD;
    sdlScancodeLookup[SDL_SCANCODE_UP] = INPUT_FORWARD;
    sdlScancodeLookup[SDL_SCANCODE_DOWN] = INPUT_BACKWARD;
    sdlScancodeLookup[SDL_SCANCODE_LEFT] = INPUT_TURNLEFT;
    sdlScancodeLookup[SDL_SCANCODE_RIGHT] = INPUT_TURNRIGHT;
    sdlScancodeLookup[SDL_SCANCODE_A] = INPUT_STRAFELEFT;
    sdlScancodeLookup[SDL_SCANCODE_D] = INPUT_STRAFERIGHT;
    sdlScancodeLookup[SDL_SCANCODE_LCTRL] = INPUT_FIRE;
    sdlScancodeLookup[SDL_SCANCODE_RCTRL] = INPUT_FIRE;
    sdlScancodeLookup[SDL_SCANCODE_RETURN] = INPUT_FIRE;
    sdlScancodeLookup[SDL_SCANCODE_SPACE] = INPUT_USE;
    sdlScancodeLookup[SDL_SCANCODE_LSHIFT] = INPUT_RUN;
    sdlScancodeLookup[SDL_SCANCODE_ESCAPE] = INPUT_TOGGLEMENU;
}

void handleSpecialKeys()
{
    if ((SDL_GetModState() & KMOD_ALT) > 0 &&
        event.key.keysym.sym == SDLK_RETURN) {
        fullscreen = 1;
    }
}

void handleKey(uint8_t state)
{
    uint8_t key = sdlScancodeLookup[event.key.keysym.scancode];
    keys[key] = state;
    keysDown[key] = state;
}

void inputUpdate()
{
    resetEvents();
    resetKeysDown();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_KEYDOWN:
            handleSpecialKeys();
            handleKey(1);
            break;
        case SDL_KEYUP:
            handleKey(0);
            break;
        }
        if (event.type == SDL_QUIT) {
            quit = 1;
        }
    }
}

int inputGetQuit() { return quit; }

int inputGetFullscreen() { return fullscreen; }

uint8_t inputGetKey(unsigned int key) { return keys[key]; }

uint8_t inputGetKeyDown(unsigned int key) { return keysDown[key]; }
