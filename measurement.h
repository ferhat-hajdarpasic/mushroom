#ifndef measurement_h
#define measurement_h
struct Measurement {
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

enum Sensor {
    PM2_5 = 0, 
    PM10, 
    NOISE, 
    SOLAR, 
    BATTERY, 
    O3, 
    S02, 
    NO2, 
    CO, 
    AMMONIA, 
    TEMPERATURE, 
    HUMIDITY, 
    BAROMETRIC_PRESSURE, 
    VOX_CO2, 
    CO2, 
    GPS
};

#endif