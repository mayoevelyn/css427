// controller.ino
// Based on Andrew Rapp example code.
// John Walter
// Thomas Dye
// Codebase for controller

#include <XBee.h>
#include <SoftwareSerial.h>

// Initialize radio object
XBee xbee = XBee();

// Setup device interconnect over serial
SoftwareSerial mySerial(64, 65); // (A10 - blue)RX, (A11 - green)TX

///////////////////////////////////////////////////////////////////////////////
// Receiver globals
// Create reusable response objects for responses we expect to handle 
XBeeResponse response = XBeeResponse();
ZBRxResponse rx64 = ZBRxResponse();

// Data from payload
uint8_t option = 0;

const int BUFFER_SIZE = 90;
char data[BUFFER_SIZE];
// End receiver globals
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Sender globals
// 64-bit addressing: This is the SH + SL address of remote XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40E3CD0F);
TxStatusResponse txStatus = TxStatusResponse();
// End sender globals
///////////////////////////////////////////////////////////////////////////////

// Setup
void setup()
{    
    // Prepare serial connections
    Serial.begin(9600);
    xbee.setSerial(Serial);

    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);  
    
    // Allow radio to fully boot and establish connection to remote
    delay(10000);

    mySerial.println("Finished booting Arduino Mega");
}

// Loop
void loop()
{
    sendPackets("1.1,2.2,3.3");
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

void sendPackets(String message)
{
    char payload[message.length()];
    strcpy(payload, message.c_str());
    
    Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));

    xbee.send(tx);   
  
    // after sending a tx request, we expect a status response
    // wait up to 5 seconds for the status response
    if (xbee.readPacket(5000))
    {
        // got a response!
        
        // should be a znet tx status                
        if (xbee.getResponse().getApiId() == TX_STATUS_RESPONSE)
        {
            xbee.getResponse().getTxStatusResponse(txStatus);
            
            // get the delivery status, the fifth byte
            if (txStatus.getStatus() == SUCCESS)
            {
                // success.
                mySerial.println("Sent: " + message);
            }
            else
            {
                // the remote XBee did not receive our packet. is it powered on?
                mySerial.println("txStatus.getStatus() != SUCCESS");
            }
        }      
    }
    else if (xbee.getResponse().isError())
    {
        mySerial.println("xbee.getResponse().isError()");        
    }
    else
    {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        mySerial.println("local XBee did not provide a timely TX Status Response.");
    }

    delay(1000);
}

