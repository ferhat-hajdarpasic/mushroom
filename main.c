#include <stdio.h>
#include "measurement.h"

char schedule[15][25] = {
    {  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    {  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    {  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8},
    {  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0},
    {  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8},
    {  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0},
    {  0,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12},
    {  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    { 12,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0},
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12},
};

char frames[] = {
    0x01, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27
};


char* sendFrame(int frameId) {
    static char frameToSend[12];
    frameToSend[0] = frames[frameId];
    if(schedule[PM2_5][frameId] != 0) {
        
    }
    return frameToSend;
}

void shift_right(unsigned char *ar, int size, int shift)
{
    int carry = 0;                              // Clear the initial carry bit.
    while (shift--) {                           // For each bit to shift ...
        for (int i = size - 1; i >= 0; --i) {   // For each element of the array from high to low ...
            int next = (ar[i] & 1) ? 0x80 : 0;  // ... if the low bit is set, set the carry bit.
            ar[i] = carry | (ar[i] >> 1);       // Shift the element one bit left and addthe old carry.
            carry = next;                       // Remember the old carry for next time.
        }   
    }
}
void shift_left(unsigned char *ar, int size, int shift)
{
    while (shift--) {                           
        int carry = 0;                              
        for (int i = size - 1; i >= 0; --i) {   
            int next = (ar[i] & 0x80) ? 0x01 : 0;
            ar[i] = carry | (ar[i] << 1);
            carry = next;
        }   
    }
}
int test_shift()
{
    unsigned char array[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0xff};
    shift_left(array, sizeof(array), 4);
    shift_left(array, sizeof(array), 4);
    shift_left(array, sizeof(array), 8);
    shift_left(array, sizeof(array), 12);
    return 0;
}

int main(int argc, char* argv) {
    for(int frameIndex = 0; frameIndex < sizeof(frames); frameIndex++) {
        char frameId = frames[frameIndex];

    }
    test_shift();
}

