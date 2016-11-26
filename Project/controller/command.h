#ifndef COMMAND_H
#define COMMAND_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "bh1750Controller.h"   // light sensor
#include "dht11Controller.h"    // temperature and humidity
#include "yl38Controller.h"     // moisture

class command
{
public:
    command(SoftwareSerial &object);
    ~command();

    char* getSensorData(byte zone);
    
private:
    SoftwareSerial *mySerial;
    
    // Controller sensors
    const byte Z1_BH1750_ADDRESS = 0x23;
    const byte Z1_DHT11_DATAPIN = 53;
    const byte Z1_YL38_ANALOGPIN = 62;    // A8 on Mega2560

    // Communication Command Codes
    const byte C_ACK = 0x00;
    const byte C_SUCCESS = 0x01;
    const byte C_FAILURE = 0x02;
    const byte C_GET_VALVE_STATE = 0x10;
    const byte C_OPEN_VALVE = 0x11;
    const byte C_CLOSE_VALVE = 0x12;
    const byte C_TOGGLE_VALVE = 0x13;
    const byte C_SET_TIME = 0x14;
    const byte C_GET_TIME = 0x15;
    const byte C_GET_ZONE_SENSORS = 0x16;
    const byte C_GET_ALL_SENSORS = 0x17;
    const byte C_GET_SCHEDULE = 0x18;
    const byte C_VALVE_DATA = 0x30;
    const byte C_TIME_DATA = 0x31;
    const byte C_SENSOR_DATA = 0x32;
    const byte C_SCHEDULE_DATA = 0x33;
    
    char* readSensors(byte zone);
    char* packPayload(byte command, char* data);
};

#endif

