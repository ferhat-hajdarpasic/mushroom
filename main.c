#include <stdio.h>
struct measurement {
    unsigned short Pm2_5;
    unsigned short Pm10;
    unsigned char Noise;
    unsigned char Solar;
    unsigned char Battery;
    unsigned short O3;
    unsigned short S02;
    unsigned short NO2;
    unsigned short CO;
    unsigned short Ammonia;
    unsigned short Temperature;
    unsigned short Humidity;
    unsigned short Barometric_Pressure;
    unsigned short VOX_CO2;
    unsigned short CO2;
    unsigned char GPS[6];
};

int main(int argc, char* argv) {
    unsigned short haba = 0xfe;
    printf("Hello world %d", 0x55);
    printf("Hello world %d", 0x55);
    printf("Hello world %d", 0x55);
    printf("Hello world %d", 0x55);
    printf("Hello world %d", 0x55);
}