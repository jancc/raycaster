#ifndef INPUT_H
#define INPUT_H

#define INPUT_MAXKEYS 11
#define INPUT_UNDEF 0
#define INPUT_FORWARD 1
#define INPUT_BACKWARD 2
#define INPUT_TURNLEFT 3
#define INPUT_TURNRIGHT 4
#define INPUT_STRAFELEFT 5
#define INPUT_STRAFERIGHT 6
#define INPUT_FIRE 7
#define INPUT_USE 8
#define INPUT_RUN 9
#define INPUT_TOGGLEMENU 10

void inputInit();
void inputUpdate();

int inputGetQuit();
int inputGetFullscreen();
uint8_t inputGetKey(unsigned int key);
uint8_t inputGetKeyDown(unsigned int key);

#endif
