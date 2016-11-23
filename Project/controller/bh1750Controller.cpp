#include "bh1750Controller.h"

// Constructor
bh1750Controller::bh1750Controller(byte address)
{
    BH1750_I2C_ADDRESS = address;
    Wire.begin();
    
    Wire.beginTransmission(address);
    Wire.write(0x10); // 1 [lux]
    Wire.endTransmission();
}

// Destructor
bh1750Controller::~bh1750Controller()
{
    // nothing to destruct
}

// Get Reading
int bh1750Controller::getReading()
{
    float value = 0;
    byte buff[2];

    if (readSensor(buff) == 2)
    {
        value = ((buff[0] << 8) | buff[1]) / 1.2;
    }

    return (int)(value, DEC);
}

// Read Sensor
byte bh1750Controller::readSensor(byte* buff)
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

