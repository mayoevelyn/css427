#include <WString.h>
#include <SoftwareSerial.h>     // console serial output
#include "Codes.h"              // command definitions
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
const unsigned int SENSOR_READ_INTERVAL = 30;            // seconds

// Setup
void setup()
{
    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    mySerial.println(F("Started booting Controller"));

    // Start radio and allow to fully boot and establish connection to remote
    radio = XBeeController(XBEE_SH_ADDRESS, XBEE_SL_ADDRESS, &mySerial);
    delay(10000);  

    // Start the real time clock
    rtc = DS3231Controller(DS3231_ADDRESS);
    mySerial.print(F("System time is "));
    mySerial.println(rtc.getTimeString());

    // Update sensors for all zones
    for (byte i = ZONE_1; i <= ZONE_1; i++)
    {
        sendPayload(processor.packSensorData(i));
    }

    // Start periodic tasks
    task.setInterval(1000, taskUpdateRTC);
    task.setInterval(SENSOR_READ_INTERVAL * 1000, taskSensors);
    task.setInterval(5000, taskCheckSchedule);
    
    mySerial.println(F("Finished booting Controller"));
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
        byte code = (byte)token.getToken(payload, ',', 0).toInt();
        byte zone;
        
        switch (payloadSize)
        {
            case 1:
                execute(code);
                break;
            case 2:
                zone = (byte)token.getToken(payload, ',', 1).toInt();
                execute(code, zone);
                break;
            default:
                execute(code, payload);
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
    sendPayload(processor.packSensorData(ZONE_1));
}

// Task Check Schedule
void taskCheckSchedule()
{
    if (processor.checkSchedule(ZONE_1, rtc.getHour(), rtc.getMinute()))
    {
        mySerial.println(F("Scheduled event:  begin irrigation for zone 1"));
        byte duration = processor.getDuration(ZONE_1);
        processor.openValve(ZONE_1);
        
        // create a one time use timer to shut off valve
        task.setTimer((long)duration * 60 * 1000, taskCloseValveZone1, 1);
    }
}

// Task Close Valve Zone 1
void taskCloseValveZone1()
{
    mySerial.println(F("Scheduled event:  end irrigation for zone 1"));
    processor.closeValve(ZONE_1);
}

// Execute command code
void execute(byte code)
{
    switch (code)
    {
        case C_GET_TIME:
            // needs to be implemented
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
void execute(byte code, String payload)
{
    switch (code)
    {
        case C_SET_TIME:
            // needs to be implemented
            break;
        case C_SET_SCHEDULE:
            sendPayload(processor.packSetSchedule(payload));
            break;
        default:
            sendPayload(String(C_ACK));
            break;
    }
}

// Send Payload
void sendPayload(String data)
{
    radio.sendData(data);

    // Block on ackSentData.  If errors occur, display error.
    radio.ackSentData(data);
}

