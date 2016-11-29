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
            mySerial->println("readSensors: error, invalid zone (" + String(zone) + ")");
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

    mySerial->println("readSensors: " + values);
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
            mySerial->println("readValve: error, invalid zone (" + String(zone) + ")");
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
            mySerial->println("readValve: error, invalid zone (" + String(zone) + ")");
            success = false;
            break;
    }
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
            mySerial->println("readValve: error, invalid zone (" + String(zone) + ")");
            success = false;
            break;
    }
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
            mySerial->println("readValve: error, invalid zone (" + String(zone) + ")");
            success = false;
            break;
    }
}
