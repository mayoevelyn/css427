#include <XBee.h>

//Receives a ZB RX packet and sets a PWM value based on packet data.

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

void setup() {
    pinMode(53, OUTPUT);
    
    Serial.begin(9600);
    xbee.begin(Serial);
}

void loop() {
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable())
    {
        // got something
        
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
        {
            // got a zb rx packet
            
            // now fill our zb rx class
            xbee.getResponse().getZBRxResponse(rx);
            
            // set dataLed to value of the first byte in the data
            digitalWrite(53, rx.getData(0));
        }
    }
}
