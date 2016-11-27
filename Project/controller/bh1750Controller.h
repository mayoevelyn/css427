#ifndef BH1750_CONTROLLER_H
#define BH1750_CONTROLLER_H

#include <arduino.h>
#include <Wire.h>

class bh1750Controller
{
public:
    bh1750Controller(byte address);
    ~bh1750Controller();

    int getReading();
    
private:
    byte BH1750_I2C_ADDRESS;
    byte buff[2];

    int readSensor();
};

#endif
