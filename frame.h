#ifndef frame_h
#define frame_h
#include "measurement.h"

extern unsigned char* frame(unsigned char frameIndex, struct Measurement* measurement );
extern char schedule[15][24];
extern char frames[24];

#endif