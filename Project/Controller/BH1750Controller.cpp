// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - BH1750 Light Sensor Wrapper
// ----------------------------------------------------------------------------

#include "BH1750Controller.h"

// Constructor
BH1750Controller::BH1750Controller(byte address)
{
    BH1750_I2C_ADDRESS = address;
    Wire.begin();
    
    Wire.beginTransmission(address);
    Wire.write(0x10); // 1 [lux]
    Wire.endTransmission();
}

// Destructor
BH1750Controller::~BH1750Controller()
{
    // nothing to destruct
}

// Get Reading
int BH1750Controller::getReading()
{
    float value = 0;

    if (readSensor() == 2)
    {
        value = ((buff[0] << 8) | buff[1]) / 1.2;
    }

    return (int)(value);
}

// Read Sensor
int BH1750Controller::readSensor()
{
    byte i = 0;
    Wire.beginTransmission(BH1750_I2C_ADDRESS);
    Wire.requestFrom(BH1750_I2C_ADDRESS, (byte)2);

    while(Wire.available())
    {
        buff[i] = Wire.read();
        i++;
    }

    Wire.endTransmission();  
    return i;
}

