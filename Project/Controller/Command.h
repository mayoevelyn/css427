// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Yang Peng
//
// Controller  - Command Logic
// ----------------------------------------------------------------------------

#ifndef COMMAND_H
#define COMMAND_H

#include <arduino.h>
#include <WString.h>
#include <SoftwareSerial.h>     // console serial output
#include "Codes.h"              // command definitions
#include "Schedule.h"           // irrigation scheduling
#include "SRD05vdcController.h" // relay
#include "BH1750Controller.h"   // light sensor
#include "DHT11Controller.h"    // temperature and humidity
#include "YL38Controller.h"     // moisture
#include "Tokenizer.h"          // string tokenizer

class Command
{
public:
    Command(SoftwareSerial *object);
    ~Command();

    String packSensorData(byte zone);
    String packValveData(byte zone);
    String packOpenValve(byte zone);
    String packCloseValve(byte zone);
    String packToggleValve(byte zone);
    String packSetSchedule(String payload);
    
    void openValve(byte zone);
    void closeValve(byte zone);

    void setSchedule(String payload);
    void setSchedule(byte zone, byte hour, byte minute, byte duration);
    byte getDuration(byte zone);
    bool checkSchedule(byte zone, byte hour, byte minute);
    
private:
    const byte TOTAL_ZONES = 1;
    
    // Controller sensors
    const byte Z1_BH1750_ADDRESS = 0x23;
    const byte Z1_SRD05VDC_DATAPIN = 52;
    const byte Z1_DHT11_DATAPIN = 53;
    const byte Z1_YL38_ANALOGPIN = 62;    // A8 on Mega2560

    SoftwareSerial *mySerial;
    Schedule event;
    SRD05vdcController z1valve = SRD05vdcController(Z1_SRD05VDC_DATAPIN);        // zone 1 valve relay

    String packPayload(byte code);
    String packPayload(byte code, String data);
    String readSensors(byte zone);
    String readValve(byte zone);
    void toggleValve(byte zone);

    bool success;
};

#endif

