#ifndef COMMAND_H
#define COMMAND_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "codes.h"              // command definitions
#include "srd05vdcController.h" // relay
#include "bh1750Controller.h"   // light sensor
#include "dht11Controller.h"    // temperature and humidity
#include "yl38Controller.h"     // moisture

class command
{
public:
    command(SoftwareSerial &object);
    ~command();

    char* packSensorData(byte zone);
    char* packValveData(byte zone);
    
private:
    // Controller sensors
    const byte Z1_BH1750_ADDRESS = 0x23;
    const byte Z1_SRD05VDC_DATAPIN = 52;
    const byte Z1_DHT11_DATAPIN = 53;
    const byte Z1_YL38_ANALOGPIN = 62;    // A8 on Mega2560
    
    SoftwareSerial *mySerial;
    srd05vdcController z1valve = srd05vdcController(Z1_SRD05VDC_DATAPIN);        // zone 1 valve relay
    
    char* readSensors(byte zone);
    char* readValve(byte zone);
    char* packPayload(byte command, char* data);

    bool success;
};

#endif

