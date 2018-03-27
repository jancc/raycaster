#include "engine.h"

int8_t selected;
uint8_t current;

void menuInit() {
    selected = 0;
    current = 0;
}

void menuBegin() {
    current = 0;
}

void menuEnd() {
    if(inputGetKeyDown(INPUT_FORWARD)) {
        selected--;
    } else if(inputGetKeyDown(INPUT_BACKWARD)) {
        selected++;
    }
    selected = clamp(selected, 0, current - 1);
}

void menuResetSelection() {
    selected = 0;
}

uint8_t menuDoButton(char * text) {
    uint8_t state = 0;
    char buf[255];
    if(current == selected) {
        snprintf(buf, 255, "> %s", text);
        if(inputGetKeyDown(INPUT_FIRE)) {
            state = 1;
        }
    } else {
        snprintf(buf, 255, "%s", text);
    }
    gfxRenderText(buf, 32, 32 + current * 16);
    current++;
    return state;
}
