// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Yang Peng
//
// Controller  - Command Logic
// ----------------------------------------------------------------------------

#include "Command.h"

// Constructor
Command::Command(SoftwareSerial *object)
{
    mySerial = object;
    success = false;
}

// Destructor()
Command::~Command()
{
    // nothing to destruct
}

// Pack Sensor Data
String Command::packSensorData(byte zone)
{
    return packPayload(C_SENSOR_DATA, readSensors(zone));
}

// Pack Valve Data
String Command::packValveData(byte zone)
{
    return packPayload(C_VALVE_DATA, readValve(zone));
}

// Pack Open Valve
String Command::packOpenValve(byte zone)
{
    openValve(zone);
    return packPayload(C_SUCCESS);
}

// Pack Close Valve
String Command::packCloseValve(byte zone)
{
    closeValve(zone);
    return packPayload(C_SUCCESS);
}

// Pack Toggle Valve
String Command::packToggleValve(byte zone)
{
    toggleValve(zone);
    return packPayload(C_SUCCESS);
}

// Pack Set Schedule
String Command::packSetSchedule(String payload)
{
    setSchedule(payload);
    return packPayload(C_SUCCESS);
}

// Pack Payload
String Command::packPayload(byte code)
{
    if (!success)
    {
        return String(C_FAILURE);
    }

    return String(code);
}

// Pack Payload
String Command::packPayload(byte code, String data)
{
    if (!success)
    {
        return String(C_FAILURE);
    }

    String payload = String(code) + "," + data;

    return payload;
}

// Read Sensors
String Command::readSensors(byte zone)
{
    byte bh1750Addr, dht11Addr, yl38Addr;

    switch (zone)
    {
        case 1:
            bh1750Addr = Z1_BH1750_ADDRESS;
            dht11Addr = Z1_DHT11_DATAPIN;
            yl38Addr = Z1_YL38_ANALOGPIN;
            break;
        default:
            mySerial->print(F("readSensors: error, invalid zone ("));
            mySerial->print(zone);
            mySerial->println(F(")"));
            success = false;
            return "";
    }
      
    // Initialize light sensor
    BH1750Controller light = BH1750Controller(bh1750Addr);
    
    // Initialize temperature and humidity sensor
    DHT11Controller thermostat = DHT11Controller(dht11Addr);
    
    // Initialize moisture sensor
    YL38Controller moisture = YL38Controller(yl38Addr);
     
    // Output string format:  light,temp,humidity,moisture
    String values = String((int)zone) + ",";
    values += String(light.getReading()) + ",";
    values += String(thermostat.getTempReading()) + ",";
    values += String(thermostat.getHumidityReading()) + ",";
    values += String(moisture.getReading());

    success = true;
    return values;
}

// Read Valve
String Command::readValve(byte zone)
{
    bool state;
    
    switch (zone)
    {
        case 1:
            state = z1valve.isOn();
            success = true;
            break;
        default:
            mySerial->print(F("readValve: error, invalid zone ("));
            mySerial->print(zone);
            mySerial->println(F(")"));
            success = false;
            return "";
    }
    
    String values = String((int)zone) + ",";
    
    if (state)
    {
        // valve is on
        values += "1";
    }
    else
    {
        // valve is off
        values += "0";
    }

    return values;
}

// Open Valve
void Command::openValve(byte zone)
{
    switch (zone)
    {
        case 1:
            z1valve.setOn();
            success = true;
            break;
        default:
            mySerial->print(F("openValve: error, invalid zone ("));
            mySerial->print(zone);
            mySerial->println(F(")"));
            success = false;
            break;
    }

    mySerial->print(F("openValve: zone "));
    mySerial->println(zone);
}

// Close Valve
void Command::closeValve(byte zone)
{
    switch (zone)
    {
        case 1:
            z1valve.setOff();
            success = true;
            break;
        default:
            mySerial->print(F("closeValve: error, invalid zone ("));
            mySerial->print(zone);
            mySerial->println(F(")"));
            success = false;
            break;
    }

    mySerial->print(F("closeValve: zone "));
    mySerial->println(zone);
}

// Toggle Valve
void Command::toggleValve(byte zone)
{
    switch (zone)
    {
        case 1:
            if (z1valve.isOn())
            {
                z1valve.setOff();
            }
            else
            {
                z1valve.setOn();
            }
            success = true;
            break;
        default:
            mySerial->print(F("toggleValve: error, invalid zone ("));
            mySerial->print(zone);
            mySerial->println(F(")"));
            success = false;
            break;
    }

    mySerial->print(F("toggleValve: zone "));
    mySerial->println(zone);
}

// Set Schedule
void Command::setSchedule(String payload)
{
    Tokenizer token;
    
    byte zone = (byte)token.getToken(payload, ',', 1).toInt();
    
    if (zone < 1 || zone > TOTAL_ZONES)
    {
        mySerial->print(F("setSchedule: error, invalid zone ("));
        mySerial->print(zone);
        mySerial->println(F(")"));
        success = false;
        return;
    }

    byte hour = (byte)token.getToken(payload, ',', 2).toInt();
    byte minute = (byte)token.getToken(payload, ',', 3).toInt();
    byte duration = (byte)token.getToken(payload, ',', 4).toInt();

    event.setSchedule(zone, hour, minute, duration);
    success = true;

    mySerial->print(F("setSchedule: "));
    mySerial->print(hour);
    mySerial->print(F(":"));
    mySerial->print(minute);
    mySerial->print(F(" for zone "));
    mySerial->print(zone);
    mySerial->print(F(", "));
    mySerial->print(duration);
    mySerial->println(F(" minute duration"));
}

// Get Duration
byte Command::getDuration(byte zone)
{
    if (zone < 1 || zone > TOTAL_ZONES)
    {
        mySerial->print(F("getDuration: error, invalid zone ("));
        mySerial->print(zone);
        mySerial->println(F(")"));
        success = false;
        return 0;
    }
    
    success = true;
    return event.getDuration(zone);
}

// Check Schedule
bool Command::checkSchedule(byte zone, byte hour, byte minute)
{
    if (zone < 1 || zone > TOTAL_ZONES)
    {
        mySerial->print(F("checkSchedule: error, invalid zone ("));
        mySerial->print(zone);
        mySerial->println(F(")"));
        success = false;
        return false;
    }

    success = true;
    return event.checkSchedule(zone, hour, minute);
}

