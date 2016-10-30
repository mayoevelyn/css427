// Test code for CSS 427 Final Project, based on Andrew Rapp example code.
// John Walter
// Thomas Dye
// Receiver codebase for command station

#include <XBee.h>

// Initialize radio object
XBee xbee = XBee();

// Create reusable response objects for responses we expect to handle 
XBeeResponse response = XBeeResponse();
ZBRxResponse rx64 = ZBRxResponse();

// LED pin assignments
int errorLed = 51;
int statusLed = 52;
int dataLed = 53;

// Data from payload
uint8_t option = 0;
char data[128];

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

    // Check LEDs
    flashLed(errorLed, 3, 50);
    flashLed(statusLed, 3, 50);
    flashLed(dataLed, 3, 50);
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
            strcpy(data, rx64.getData(0));
            
            // flash RX indicator for each byte in payload  
            flashLed(statusLed, 4, 25);

            // process received data
            setDataLed();
        }
        else
        {
            // not something we were expecting
            flashLed(errorLed, 1, 25);    
        }
    }
    else if (xbee.getResponse().isError())
    {
        //nss.print("Error reading packet.  Error code: ");  
        //nss.println(xbee.getResponse().getErrorCode());
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
    if (data == "hello world")
    {
        digitalWrite(dataLed, LOW);
    }
    else
    {
        digitalWrite(dataLed, HIGH);
    }
}

