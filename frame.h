#ifndef frame_h
#define frame_h
#include "measurement.h"

extern unsigned char* frame(unsigned char frameIndex, struct Measurement* measurement );
extern char schedule[15][25];
extern char frames[25];

#endif