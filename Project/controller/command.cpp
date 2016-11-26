#include "command.h"

// Constructor
command::command(SoftwareSerial &object)
{
    *mySerial = object;
}

// Destructor()
command::~command()
{
    // nothing to destruct
}

// Get Sensor Data
char* command::getSensorData(byte zone)
{
    return packPayload(C_SENSOR_DATA, readSensors(zone));
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
            mySerial->println("getSensorData: error, invalid zone (" + String(zone) + ")");
            return data;
    }
    
    // Initialize light sensor
    bh1750Controller light = bh1750Controller(bh1750Addr);
    
    // Initialize temperature and humidity sensor
    dht11Controller thermostat = dht11Controller(dht11Addr);
    
    // Initialize moisture sensor
    yl38Controller moisture = yl38Controller(yl38Addr);

    // Output string format:  zone,light,temp,humidity,moisture
    String output = String(zone);
    output += ",";
    output += String(light.getReading());
    output += ",";
    output += String(thermostat.getTempReading());
    output += ",";
    output += String(thermostat.getHumidityReading());
    output += ",";
    output += String(moisture.getReading());

    mySerial->println("getSensorData: " + output);

    char data[output.length()];
    strcpy(data, output.c_str());
    return data;
}

// Pack Payload
char* command::packPayload(byte code, char* data)
{
    byte dataLength = sizeof(data);

    char payload[dataLength + 1];

    payload[0] = code;
    for (int i = 0; i < dataLength; i++)
    {
        payload[i + 1] = data[i];
    }

    return payload;
}
