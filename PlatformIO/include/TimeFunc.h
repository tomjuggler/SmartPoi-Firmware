#ifndef TIMEFUNC_H
#define TIMEFUNC_H

#include <Arduino.h>

// Extern declarations for shared variables
extern unsigned long previousMillis3;
extern long interval;
extern int imageToUse;
extern int maxImages;
extern int minImages;
extern int pattern;

void ChangePatternPeriodically();

#endif
