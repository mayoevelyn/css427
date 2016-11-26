#include "command.h"

// Constructor
command::command(SoftwareSerial &object)
{
    *mySerial = object;
    success = false;
}

// Destructor()
command::~command()
{
    // nothing to destruct
}

// Get Sensor Data
char* command::packSensorData(byte zone)
{
    success = false; // reset
    return packPayload(C_SENSOR_DATA, readSensors(zone));
}

char* command::packValveData(byte zone)
{
    success = false; // reset
    return packPayload(C_VALVE_DATA, readValve(zone));
}

// Read Sensors
char* command::readSensors(byte zone)
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
            char data[0];
            mySerial->println("readSensors: error, invalid zone (" + String(zone) + ")");
            success = false;
            return data;
    }
    
    // Initialize light sensor
    bh1750Controller light = bh1750Controller(bh1750Addr);
    
    // Initialize temperature and humidity sensor
    dht11Controller thermostat = dht11Controller(dht11Addr);
    
    // Initialize moisture sensor
    yl38Controller moisture = yl38Controller(yl38Addr);

    // Output string format:  light,temp,humidity,moisture
    String values = String(light.getReading());
    values += ",";
    values += String(thermostat.getTempReading());
    values += ",";
    values += String(thermostat.getHumidityReading());
    values += ",";
    values += String(moisture.getReading());

    mySerial->println("getSensorData: " + values);

    String output = String(zone);
    output += values;
    
    char data[output.length()];
    strcpy(data, output.c_str());
    success = true;
    return data;
}

// Read Valve
char* command::readValve(byte zone)
{
    bool state;
    
    switch (zone)
    {
        case 1:
            state = z1valve.isOn();
            success = true;
            break;
        default:
            char data[0];
            mySerial->println("readValve: error, invalid zone (" + String(zone) + ")");
            success = false;
            return data;
    }
    
    char payload[2];
    payload[0] = zone;
    success = true;
    
    if (state)
    {
        // valve is on
        return payload[1] = 1;
    }
    else
    {
        // valve is off
        return payload[1] = 0;
    }
}

// Pack Payload
char* command::packPayload(byte code, char* data)
{
    if (!success)
    {
        return 0x02;
    }
    
    byte dataLength = sizeof(data);

    char payload[dataLength + 1];

    payload[0] = code;
    for (int i = 0; i < dataLength; i++)
    {
        payload[i + 1] = data[i];
    }

    return payload;
}
