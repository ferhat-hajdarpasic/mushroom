#include <stdio.h>
#include "measurement.h"
#include "frame.h"

void LOG(const char* msg) {
    printf("%s", msg);
    printf("\n");
    fflush(stdout);
}

void LOGBUFFER(const unsigned char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        printf("%02x", buffer[i]);
    }
    printf("\n");
    fflush(stdout);
}

int main(int argc, char* argv) {
    struct Measurement m = {.Pm2_5 = 0x0fff};
    for(int frameIndex = 0; frameIndex < sizeof(frames); frameIndex++) {
        LOG("Frame:");
        unsigned char* result = frame(frameIndex, &m);
        LOGBUFFER(result, 12);
    }
    printf("Finished\n");
}