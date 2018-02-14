#ifndef MENU_H
#define MENU_H

/**
 * Simple immediate mode like menu.
 * Use in between gfxBegin and gfxEnd.
 * Call menuDo* functions inside a menuBegin and menuEnd block.
 */

#include <stdint.h>

void menuInit();
void menuBegin();
void menuEnd();
void menuResetSelection();

uint8_t menuDoButton(char * text);

#endif