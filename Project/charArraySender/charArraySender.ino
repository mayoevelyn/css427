// Test code for CSS 427 Final Project, based on Andrew Rapp example code.
// John Walter
// Thomas Dye
// Sender codebase for controller

#include <XBee.h>

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

// Setup
void setup()
{
    // Prepare diagnostic leds
    pinMode(statusLed, OUTPUT);
    pinMode(errorLed, OUTPUT);
    
    // Prepare serial connections
    Serial.begin(9600);
    delay(250);
    Serial2.begin(9600);
    xbee.setSerial(Serial);
    
    // Allow radio to fully boot and establish connection to remote
    delay(10000);

    // Check LEDs
    flashLed(errorLed, 3, 50);
    flashLed(statusLed, 3, 50);
}

// Loop
void loop()
{
    
    packHello();
    Tx64Request zbtx = Tx64Request(addr64, (uint8_t *)buffer, strlen(buffer));
    xbee.send(zbtx);
    delay(2000);

    packGoodbye();
    zbtx = Tx64Request(addr64, (uint8_t *)buffer, strlen(buffer));
    xbee.send(zbtx);
    delay(2000);

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
    delay(1000);
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

void packHello()
{
    buffer[0] = 'h';
    buffer[1] = 'e';
    buffer[2] = 'l';
    buffer[3] = 'l';
    buffer[4] = '0';
    buffer[5] = '\r';
}

void packGoodbye()
{
    buffer[0] = 'g';
    buffer[1] = 'o';
    buffer[2] = 'o';
    buffer[3] = 'd';
    buffer[4] = 'b';
    buffer[5] = 'y';
    buffer[6] = 'e';
    buffer[7] = '\r';
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

