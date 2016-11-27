#ifndef COMMAND_H
#define COMMAND_H

#include <arduino.h>
#include <SoftwareSerial.h>     // console serial output
#include "codes.h"              // command definitions
#include "srd05vdcController.h" // relay
#include "bh1750Controller.h"   // light sensor
#include "dht11Controller.h"    // temperature and humidity
#include "yl38Controller.h"     // moisture

class command
{
public:
    command(SoftwareSerial *object);
    ~command();

    String packSensorData(byte zone);
    String packValveData(byte zone);
    String packOpenValve(byte zone);
    String packCloseValve(byte zone);
    String packToggleValve(byte zone);
    
private:
    // Controller sensors
    const byte Z1_BH1750_ADDRESS = 0x23;
    const byte Z1_SRD05VDC_DATAPIN = 52;
    const byte Z1_DHT11_DATAPIN = 53;
    const byte Z1_YL38_ANALOGPIN = 62;    // A8 on Mega2560

    SoftwareSerial *mySerial;
    srd05vdcController z1valve = srd05vdcController(Z1_SRD05VDC_DATAPIN);        // zone 1 valve relay
    
    String readSensors(byte zone);
    String readValve(byte zone);
    void openValve(byte zone);
    void closeValve(byte zone);
    void toggleValve(byte zone);
    String packPayload(byte code);
    String packPayload(byte code, String data);

    bool success;
};

#endif

