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
uint8_t data;

// Setup
void setup()
{
    initBuffer();
    
    // Prepare diagnostic leds
    pinMode(errorLed, OUTPUT);
    pinMode(statusLed, OUTPUT);
    pinMode(dataLed,  OUTPUT);
    
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

    // Check LEDs
    for (int i = 0; i < 10; i++)
    {
        flashLed(errorLed, 1, 100);
        flashLed(statusLed, 1, 100);
        flashLed(dataLed, 1, 100);
    }
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
            data = rx64.getData(0);
            
            // flash RX indicator for each byte in payload  
            flashLed(statusLed, 1, 100);

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
        Serial.print("Error reading packet.  Error code: ");  
        Serial.println(xbee.getResponse().getErrorCode());
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
    if (data == "hello")
    {
        digitalWrite(dataLed, LOW);
    }
    else if (data == "goodbye")
    {
        digitalWrite(dataLed, HIGH);
    }
}

void initBuffer()
{
    for (int i = 0; i < 100; i++)
    {
        //data[i] = '0';
    }
}

