#include <stdio.h>
#include "measurement.h"
#include "frame.h"

void LOG(const char *msg)
{
    printf("%s", msg);
    printf("\n");
    fflush(stdout);
}

void LOGBUFFER(const unsigned char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%02x", buffer[i]);
    }
    printf("\n");
    fflush(stdout);
}

int mainSigfox(int argc, char *argv)
{
    struct Measurement m =
    {
        .Pm2_5 = 1151,
        .Pm10 = 1152,
        .Noise = 153,
        .Solar = 154,
        .Battery =155,
        .O3 =156,
        .S02 =157,
        .NO2 =158,
        .CO =159,
        .Ammonia =160,
        .Temperature =161,
        .Humidity =162,
        .Barometric_Pressure =163,
        .VOX_CO2 =164,
        .CO2 =165
    };

    for (int frameIndex = 0; frameIndex < sizeof(frames); frameIndex++)
    {
        LOG("Frame:");
        unsigned char *result = frame(frameIndex, &m);
        LOGBUFFER(result, 12);
    }
    LOG("Finished\n");

    return 0;
}