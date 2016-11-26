#ifndef COMMAND_H
#define COMMAND_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "codes.h"              // command definitions
#include "bh1750Controller.h"   // light sensor
#include "dht11Controller.h"    // temperature and humidity
#include "yl38Controller.h"     // moisture

class command
{
public:
    command(SoftwareSerial &object);
    ~command();

    char* packSensorData(byte zone);
    
private:
    SoftwareSerial *mySerial;
    
    // Controller sensors
    const byte Z1_BH1750_ADDRESS = 0x23;
    const byte Z1_DHT11_DATAPIN = 53;
    const byte Z1_YL38_ANALOGPIN = 62;    // A8 on Mega2560
    
    char* readSensors(byte zone);
    char* packPayload(byte command, char* data);
};

#endif

