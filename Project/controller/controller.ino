#include <SoftwareSerial.h>     // console serial output
//#include "ds3231Controller.h"   // real time clock
#include "xbeeController.h"     // radio
#include "command.h"            // command processor
#include "codes.h"              // command definitions

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
xbeeController radio;                                   // start radio communication
//ds3231Controller rtc(DS3231_ADDRESS);                   // start real time clock
command processor(&mySerial);                           // start command processor

// Global Variables
const unsigned int SENSOR_READ_INTERVAL = 6;            // seconds
unsigned long previousMillis;

// Setup
void setup()
{
    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    mySerial.println("Starting booting Controller");

    // Start radio and allow to fully boot and establish connection to remote
    radio = xbeeController(XBEE_SH_ADDRESS, XBEE_SL_ADDRESS, &mySerial);
    delay(10000);  

    // Initialize globals
    previousMillis = 0;

    mySerial.println("Finished booting Controller");
}

// Loop
void loop()
{
    // Get snapshot of time
    unsigned long currentMillis = millis();

    // Use a non-blocking delay for a periodic timed event
    if ((unsigned long)(currentMillis - previousMillis) >= SENSOR_READ_INTERVAL * 1000)
    {
        //mySerial.println("debug: inside loop");
        execute(C_SENSOR_DATA, ZONE_1);
        
        // Use the snapshot to set track time until next event
        previousMillis = currentMillis;
    }

    String payload = radio.receiveData();
    if (payload != 0)
    {
        mySerial.println(payload);
    }

//    // Poll for incoming transmissions.
//    if (radio.receiveData())
//    {
//        mySerial.println(radio.getLastMessage());
//    }

//    if (radio.hasData())
//    {
//        char* payload = radio.getData();
//        //execute(payload);
//    }
}

// Execute command code
void execute(byte code)
{
    switch (code)
    {
        case C_ACK:
        case C_SUCCESS:
        case C_FAILURE:
        case C_TIME_DATA:
            break;
    }
}

// Execute command code for zone
void execute(byte code, byte zone)
{
    switch (code)
    {
        case C_VALVE_DATA:
            //sendPayload(processor.packValveData(zone));
            break;
        case C_SENSOR_DATA:
            //mySerial.println("begin sendPayload");
            sendPayload(processor.packSensorData(zone));
            //mySerial.println("end sendPayload");
            break;
        case C_SCHEDULE_DATA:
            break;
    }
}

// Execute received command code
void execute(char* payload)
{
    byte zone;

    switch (payload[0])
    {
        case C_GET_VALVE_STATE:
            //execute(C_VALVE_DATA, payload[1]);
            break;
        case C_OPEN_VALVE:
        case C_CLOSE_VALVE:
        case C_TOGGLE_VALVE:
        case C_SET_TIME:
        case C_GET_TIME:
        case C_GET_ZONE_SENSORS:
            //execute(C_SENSOR_DATA, payload[1]);
            break;
        case C_GET_ALL_SENSORS:
//            // Adjust endpoint to reflect last zone
//            for (byte i = ZONE_1; i <= ZONE_1; i++)
//            {
//                execute(C_SENSOR_DATA, i);
//            }
            break;
        case C_GET_SCHEDULE:
            break;
    }
}

// Send Payload
void sendPayload(String data)
{
    //mySerial.println("debug: in sendPayload");
    radio.sendData(data);
    //mySerial.println("debug: finished sending");

    // Block on ackSentData.  If errors occur, display error.
    radio.ackSentData(data);
}

