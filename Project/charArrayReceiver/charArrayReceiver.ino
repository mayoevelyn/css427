// Test code for CSS 427 Final Project, based on Andrew Rapp example code.
// John Walter
// Thomas Dye
// Receiver codebase for command station

#include <XBee.h>
#include <SoftwareSerial.h>

// Initialize radio object
XBee xbee = XBee();

// Setup device interconnect over serial
SoftwareSerial mySerial(64, 65); // (A10 - blue)RX, (A11 - green)TX

// Create reusable response objects for responses we expect to handle 
XBeeResponse response = XBeeResponse();
ZBRxResponse rx64 = ZBRxResponse();

// LED pin assignments
int errorLed = 51;
int statusLed = 52;
int dataLed = 53;

// Data from payload
uint8_t option = 0;

const int BUFFER_SIZE = 90;
char data[BUFFER_SIZE];

// Setup
void setup()
{
    // Prepare diagnostic leds
    pinMode(errorLed, OUTPUT);
    pinMode(statusLed, OUTPUT);
    pinMode(dataLed,  OUTPUT);
    
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);

    // Check LEDs
    for (int i = 0; i < 10; i++)
    {
        flashLed(errorLed, 1, 100);
        flashLed(statusLed, 1, 100);
        flashLed(dataLed, 1, 100);
    }
    mySerial.println("Finished booting Arduino Mega");
}

// Loop
void loop()
{
    // continuously reads packets
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable())
    {
        // got something
        
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        {
            // got a rx packet
            
            xbee.getResponse().getZBRxResponse(rx64);
            option = rx64.getOption();

            // read in each byte of the incoming data
            for (int i = 0; i < rx64.getDataLength(); i++)
            {
                if (i < BUFFER_SIZE)
                {
                    data[i] = rx64.getData(i);
                }
            }
            
            // flash RX indicator for each byte in payload  
            flashLed(statusLed, 1, 100);

            // process received data
            setDataLed();
        }
        else
        {
            // not something we were expecting
            mySerial.println("Error ZB_RX_RESPONSE: format not expected");
            flashLed(errorLed, 1, 25);
        }
    }
    else if (xbee.getResponse().isError())
    {
        mySerial.print("Error reading packet.  Error code: ");  
        mySerial.println(xbee.getResponse().getErrorCode());
        flashLed(errorLed, 3, 100);
    }
}

// Flash LED
void flashLed(int pin, int times, int wait)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(pin, HIGH);
        delay(wait);
        digitalWrite(pin, LOW);
        
        if (i + 1 < times)
        {
            delay(wait);
        }
    }
}

// Set Data LED
void setDataLed()
{
    String eval = String(data);
    mySerial.println("Received: " + eval);
    
    if (eval == "goodbye")
    {
        digitalWrite(dataLed, LOW);
    }
    else if (eval == "hello")
    {
        digitalWrite(dataLed, HIGH);
    }
}

