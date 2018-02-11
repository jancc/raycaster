#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
struct ent;

void worldCreateTilemap(uint32_t width, uint32_t height);
void worldRandomize();
void worldSetTile(uint32_t x, uint32_t y, uint8_t value);
uint8_t worldGetTile(uint32_t x, uint32_t y);
uint8_t worldGetCollision(uint32_t x, uint32_t y);
uint8_t worldGetCollisionInArea(double x, double y, double w, double h);
void worldUpdate();
void worldClear();

#endif