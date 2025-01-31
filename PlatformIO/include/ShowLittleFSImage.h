#ifndef SHOW_LITTLEFS_IMAGE_H
#define SHOW_LITTLEFS_IMAGE_H

#include "Globals.h"

extern int cnti;
extern String bin;
extern int imageToUse;
extern int pxAcross;
extern int pxDown;
extern uint8_t message1Data[MAX_PX];
extern File a;

void showLittleFSImage();

#endif
