#ifndef BH1750_CONTROLLER_H
#define BH1750_CONTROLLER_H

#include <arduino.h>
#include <Wire.h>

class BH1750Controller
{
public:
    BH1750Controller(byte address);
    ~BH1750Controller();

    int getReading();
    
private:
    byte BH1750_I2C_ADDRESS;
    byte buff[2];

    int readSensor();
};

#endif
