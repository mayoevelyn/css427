// Test code for CSS 427 Final Project, based on Andrew Rapp example code.
// John Walter
// Thomas Dye
// Sender codebase for controller

#include <XBee.h>

// Create a 4-byte payload for an integer.
//uint8_t payload[] = { 0, 0, 0, 0 };
char helloworld[] = "Hello World\r";
char buffer[128];

// Initialize radio object
XBee xbee = XBee();

// 64-bit addressing: This is the SH + SL address of remote XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40E3CD0F);
//Tx64Request tx = Tx64Request(addr64, payload, sizeof(payload));
TxStatusResponse txStatus = TxStatusResponse();

// LED pin assignments
int statusLed = 47;
int errorLed = 53;

// Data for payload
//int output = 0;

// Setup
void setup()
{
    // Prepare diagnostic leds
    pinMode(statusLed, OUTPUT);
    pinMode(errorLed, OUTPUT);
    
    // Prepare serial connections
    Serial.begin(9600);
    Serial3.begin(9600);
    //xbee.setSerial(Serial);
    xbee.setSerial(Serial3);
    
    // Allow radio to fully boot and establish connection to remote
    delay(15000);

    // Check LEDs
    flashLed(errorLed, 3, 50);
    flashLed(statusLed, 3, 50);
}

// Loop
void loop()
{
    // Pack 4-byte integer into payload using offsets
//    payload[0] = output & 0xff;
//    payload[1] = output >> 8 & 0xff;
//    payload[2] = output >> 16 & 0xff;
//    payload[3] = output >> 24 & 0xff;

    ZBTxRequest zbtx = ZBTxRequest(addr64, (uint8_t *)helloworld, strlen(helloworld));
    xbee.send(zbtx);
    delay(20000);
    strcpy(buffer, "goodbye world\r");
    xbee.send(zbtx);
    delay(20000);

    Serial.println("using serial println");

    // Send payload and flash activity light
    //xbee.send(tx);
    
    // flash TX indicator for each byte in payload
    flashLed(statusLed, 4, 25);
  
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
                flashLed(statusLed, 1, 100);
            }
            else
            {
                // the remote XBee did not receive our packet. is it powered on?
                flashLed(errorLed, 3, 500);
            }
        }      
    }
    else if (xbee.getResponse().isError())
    {
        //nss.print("Error reading packet.  Error code: ");  
        //nss.println(xbee.getResponse().getErrorCode());
        flashLed(errorLed, 3, 100);
    }
    else
    {
        // local XBee did not provide a timely TX Status Response.  Radio is not configured properly or connected
        flashLed(errorLed, 2, 50);
    }

    //setOutput();
    //delay(1000);
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

//// Set Output
//void setOutput()
//{
//    if (output == 0)
//    {
//        output = 1;
//    }
//    else
//    {
//        output = 0;
//    }
//}

