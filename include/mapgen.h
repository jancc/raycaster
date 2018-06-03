#ifndef MAPGEN_H
#define MAPGEN_H

#include <stdint.h>

void mapgenGenerate(uint32_t width, uint32_t height, uint32_t seed);
uint32_t mapgenGetPlayerX();
uint32_t mapgenGetPlayerY();

#endif
