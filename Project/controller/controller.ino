#include <SoftwareSerial.h>     // console serial output
#include "ds3231Controller.h"   // real time clock
#include "bh1750Controller.h"   // light sensor
#include "dht11Controller.h"    // temperature and humidity
#include "yl38Controller.h"     // moisture
#include "srd05vdcController.h" // relay
#include "xbeeController.h"     // radio

// Controller components
const int XBEE_SH_ADDRESS = 0x0013A200;
const int XBEE_SL_ADDRESS = 0x40E3CD0F;
const byte DS3231_ADDRESS = 0x68;
const byte SS_RX = 64;               // A10 on Mega2560
const byte SS_TX = 65;               // A11 on Mega2560

// Controller sensors
const byte Z1_BH1750_ADDRESS = 0x23;
const byte Z1_SRD05VDC_DATAPIN = 52;
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

// Zones
const byte ZONE_1 = 1;

// Globals Objects
SoftwareSerial mySerial(SS_RX, SS_TX);                  // interconnect over serial
xbeeController radio(XBEE_SH_ADDRESS, XBEE_SL_ADDRESS); // primary communication
ds3231Controller rtc(DS3231_ADDRESS);                   // real time clock
srd05vdcController z1valve(Z1_SRD05VDC_DATAPIN);        // zone 1 valve relay

// Global Variables
const int TIME_INTERVAL = 6000;
unsigned long previousMillis;

// Setup
void setup()
{
    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);

    // Initialize globals
    previousMillis = 0;

    mySerial.println("Finished booting Controller");
}

// Loop
void loop()
{
    // Get snapshot of time
    unsigned long currentMillis = millis();

    // How much time has passed, accounting for rollover with subtraction
    if ((unsigned long)(currentMillis - previousMillis) >= TIME_INTERVAL)
    {
        char payload = packPayload(C_SENSOR_DATA, getSensorData(ZONE_1));
        
        // Use the snapshot to set track time until next event
        previousMillis = currentMillis;
    }
}

// Pack Payload
char* packPayload(byte command, char* data)
{
    byte dataLength = sizeof(data);

    char payload[dataLength + 1];

    payload[0] = command;
    for (int i = 0; i < dataLength; i++)
    {
        payload[i + 1] = data[i];
    }

    return payload;
}

// Get Sensor Data
char* getSensorData(byte zone)
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
            mySerial.println("getSensorData: error, invalid zone (" + String(zone) + ")");
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

    mySerial.println("getSensorData: " + output);

    char data[output.length()];
    strcpy(data, output.c_str());
    return data;
}

