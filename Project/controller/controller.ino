#include <SoftwareSerial.h>
#include "xbeeController.h"

// Intialize xbee controller object
xbeeController radio;

// Setup device interconnect over serial
SoftwareSerial mySerial(64, 65); // (A10 - blue)RX, (A11 - green)TX

// Constants
const int interval = 6000;

// Globals
unsigned long previousMillis;
bool toggle; // diagnostic toggle

// Setup
void setup()
{
    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
    mySerial.println("Started booting Arduino Mega");    

    // Start radio and allow to fully boot and establish connection to remote
    radio = xbeeController();
    delay(10000);

    // Initialize globals
    previousMillis = 0;
    toggle = false;

    mySerial.println("Finished booting Arduino Mega");
}

// Loop
void loop()
{
    // Get snapshot of time
    unsigned long currentMillis = millis();

    // How much time has passed, accounting for rollover with subtraction
    if ((unsigned long)(currentMillis - previousMillis) >= interval)
    {
        // Send diagnostic message
        radio.sendData(getMessage());
        mySerial.println(radio.getLastMessage());

        // If errors occur, display error
        if (!radio.ackSentData())
        {
            mySerial.println(radio.getLastMessage());
        }

        // Use the snapshot to set track time until next event
        previousMillis = currentMillis;
    }

    // If receiving data from remote, toggle message
    if (radio.receiveData())
    {
        toggle = !toggle;
    }

    if (radio.hasMessage())
    {
        mySerial.println(radio.getLastMessage());
    }
}

// Get Message
String getMessage()
{
    if (toggle)
    {
        return "1.1,2.2,3.3";
    }
    else
    {
        return "2500,2500";
    }
}

