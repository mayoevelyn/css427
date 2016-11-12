// charArrayReceiver
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

// Data from payload
uint8_t option = 0;

const int BUFFER_SIZE = 90;
char data[BUFFER_SIZE];

// Setup
void setup()
{    
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);    
    mySerial.println("Finished booting Arduino Mega");
}

// Loop
void loop()
{
    readPackets();
}

void readPackets()
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
            
            String eval = String(data);
            mySerial.println("Received: " + eval);
        }
        else
        {
            // not something we were expecting
            mySerial.println("Error ZB_RX_RESPONSE: format not expected");           
        }
    }
    else if (xbee.getResponse().isError())
    {
        mySerial.print("Error reading packet.  Error code: ");  
        mySerial.println(xbee.getResponse().getErrorCode());        
    }
}



