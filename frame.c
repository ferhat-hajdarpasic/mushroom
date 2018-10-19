#include <frame.h>
#include <measurement.h>

char schedule[15][24] = {
    {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    { 8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8},
    { 8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0},
    { 0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8,  0,  8},
    {12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0},
    { 0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12,  0,  0, 12, 12},
    {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0},
    {12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12},
    { 0,  0,  0,  0, 12, 12, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, 12, 12},
};

char frames[24] = {
    0x01, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26
};

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

unsigned short take(struct Measurement* m, int i) {
    switch(i) {
        case 0:
            return m->Pm2_5;
        case 1:
            return m->Pm10;
        case 2:
            return m->Noise;
        case 3:
            return m->Solar;
        case 4:
            return m->Battery;
        case 5:
            return m->O3;
        case 6:
            return m->S02;
        case 7:
            return m->NO2;
        case 8:
            return m->CO;
        case 9:
            return m->Ammonia;
        case 10:
            return m->Temperature;
        case 11:
            return m->Humidity;
        case 12:
            return m->Barometric_Pressure;
        case 13:
            return m->VOX_CO2;
        case 14:
            return m->CO2;
        case 15:
            return -1;
        default:
            return -1;
    }
}

void push(unsigned char* buffer, int sizeOfBuffer, unsigned short value, int bits) {
    shift_left(buffer, sizeOfBuffer, bits);
    buffer[sizeOfBuffer - 1] = (unsigned char) value;
    if(bits == 12) {
        buffer[sizeOfBuffer - 2] = buffer[sizeOfBuffer - 2] | (value >> 8) & 0x0f;
    }
}

unsigned char* frame(unsigned char frameIndex, struct Measurement* measurement ) {
    static unsigned char frameToSend[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i = 0; i < sizeof(frameToSend); i++) {
        frameToSend[i] = 0;
    }
    push(frameToSend, sizeof(frameToSend), frames[frameIndex], 8);
    for(int i = 0; i < sizeof(schedule)/sizeof(frames); i++) {
        int bits = schedule[i][frameIndex];
        if(bits > 0) {
            unsigned short value = take(measurement, i);
            push(frameToSend, sizeof(frameToSend), value, bits);
        }
    }
    return frameToSend;
}