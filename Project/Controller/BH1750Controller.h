// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Yang Peng
//
// Controller  - BH1750 Light Sensor Wrapper
// ----------------------------------------------------------------------------

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
