#include "Codes.h"              // command definitions
#include <SoftwareSerial.h>     // console serial output
#include "DS3231Controller.h"   // real time clock
#include "XBeeController.h"     // radio
#include "Command.h"            // command processor
#include "SimpleTimer.h"        // task scheduler
#include "Tokenizer.h"          // string tokenizer

// Controller components
const long XBEE_SH_ADDRESS = 0x0013A200;
const long XBEE_SL_ADDRESS = 0x40E3CD0F;
const byte DS3231_ADDRESS = 0x68;
const byte SS_RX = 64;               // A10 on Mega2560
const byte SS_TX = 65;               // A11 on Mega2560

// Zones, should be defined sequentially
const byte ZONE_1 = 1;

// Globals Objects
SoftwareSerial mySerial(SS_RX, SS_TX);                  // start interconnect over serial
DS3231Controller rtc;                                   // start real time clock
XBeeController radio;                                   // start radio communication
Command processor(&mySerial);                           // start command processor
SimpleTimer task;                                       // start task scheduler

// Global Variables
const unsigned int SENSOR_READ_INTERVAL = 6;            // seconds
byte rtcTaskID;
byte sensorTaskID;

// Setup
void setup()
{
    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    mySerial.println("Starting booting Controller");

    // Start radio and allow to fully boot and establish connection to remote
    radio = XBeeController(XBEE_SH_ADDRESS, XBEE_SL_ADDRESS, &mySerial);
    delay(10000);  

    // Start the real time clock
    rtc = DS3231Controller(DS3231_ADDRESS);
    mySerial.println("System time is " + rtc.getTimeString());

    // Update sensors for all zones
    for (byte i = ZONE_1; i <= ZONE_1; i++)
    {
        //sendPayload(processor.packSensorData(i));
    }

    // Start periodic tasks
    rtcTaskID = task.setInterval(1000, taskUpdateRTC);
    sensorTaskID = task.setInterval(SENSOR_READ_INTERVAL * 1000, taskSensors);
    
    mySerial.println("Finished booting Controller");
}

// Loop
void loop()
{
    // Process timers
    task.run();
    
    // Listen on radio for incoming data
    String payload = radio.receiveData();
    if (payload != 0)
    {
        Tokenizer token;

        byte payloadSize = token.getTokenCount(payload, ',');
        byte code;
        byte zone;
        
        switch (payloadSize)
        {
            case 1:
                code = (byte)token.getToken(payload, ',', 0).toInt();
                execute(code);
                break;
            case 2:
                code = (byte)token.getToken(payload, ',', 0).toInt();
                zone = (byte)token.getToken(payload, ',', 1).toInt();
                execute(code, zone);
                break;
            default:
                execute(payload, payloadSize);
                break;
        }
    }
}

// Task Update RTC
void taskUpdateRTC()
{
    rtc.updateTime();
}

// Task Sensors
void taskSensors()
{
    //sendPayload(processor.packSensorData(ZONE_1));
}

// Execute command code
void execute(byte code)
{
    switch (code)
    {
        case C_GET_TIME:
            break;
        case C_GET_ALL_SENSORS:
            // Adjust endpoint to reflect last zone
            for (byte i = ZONE_1; i <= ZONE_1; i++)
            {
                sendPayload(processor.packSensorData(i));
            }
            break;
        default:
            sendPayload(String(C_ACK));
            break;
    }
}

// Execute command code for zone
void execute(byte code, byte zone)
{
    switch (code)
    {
        case C_GET_VALVE_STATE:
            sendPayload(processor.packValveData(zone));
            break;
        case C_SET_OPEN_VALVE:
            sendPayload(processor.packOpenValve(zone));
            break;
        case C_SET_CLOSE_VALVE:
            sendPayload(processor.packCloseValve(zone));
            break;
        case C_SET_TOGGLE_VALVE:
            sendPayload(processor.packToggleValve(zone));
            break;
        case C_GET_ZONE_SENSORS:
            sendPayload(processor.packSensorData(zone));
            break;
        default:
            sendPayload(String(C_ACK));
            break;
    }
}

// Execute received command code
void execute(String payload, byte payloadSize)
{
//    byte code;
//    byte zone;
//
//    switch (payload[0])
//    {
//        case C_SET_TIME:
//            break;
//        case C_SET_SCHEDULE:
//            break;
//    }
}

// Send Payload
void sendPayload(String data)
{
    radio.sendData(data);

    // Block on ackSentData.  If errors occur, display error.
    radio.ackSentData(data);
}

