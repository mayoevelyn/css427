#include <SoftwareSerial.h>     // console serial output
#include "ds3231Controller.h"   // real time clock
#include "xbeeController.h"     // radio
#include "srd05vdcController.h" // relay
#include "command.h"            // command processor

// Controller components
const int XBEE_SH_ADDRESS = 0x0013A200;
const int XBEE_SL_ADDRESS = 0x40E3CD0F;
const byte DS3231_ADDRESS = 0x68;
const byte SS_RX = 64;               // A10 on Mega2560
const byte SS_TX = 65;               // A11 on Mega2560
const byte Z1_SRD05VDC_DATAPIN = 52;

// Zones
const byte ZONE_1 = 1;

// Globals Objects
static SoftwareSerial mySerial(SS_RX, SS_TX);           // interconnect over serial
xbeeController radio(XBEE_SH_ADDRESS, XBEE_SL_ADDRESS); // primary communication
ds3231Controller rtc(DS3231_ADDRESS);                   // real time clock
srd05vdcController z1valve(Z1_SRD05VDC_DATAPIN);        // zone 1 valve relay

// Global Variables
const unsigned int SENSOR_READ_INTERVAL = 6;            // seconds
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

    // Use a non-blocking delay for a periodic timed event
    if ((unsigned long)(currentMillis - previousMillis) >= SENSOR_READ_INTERVAL * 1000)
    {
        sendSensorData(ZONE_1);
        
        // Use the snapshot to set track time until next event
        previousMillis = currentMillis;
    }

    // Poll for incoming transmissions.
    if (radio.receiveData())
    {
        mySerial.println(radio.getLastMessage());
    }

    if (radio.hasData())
    {
        char* payload = radio.getData();
    }
}

// Send Sensor Data
void sendSensorData(byte zone)
{
    command processor(mySerial);

    // Pack and send sensor payload
    char* payload = processor.getSensorData(zone);
    radio.sendData(payload);
    mySerial.println(radio.getLastMessage());

    // Block on ackSentData.  If errors occur, display error.
    if (!radio.ackSentData())
    {
        mySerial.println(radio.getLastMessage());
    }
}

