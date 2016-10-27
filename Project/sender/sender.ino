#include <XBee.h>

//Sends a ZB TX request with the value of analogRead(pin5) and checks the
//status response for success

// create the XBee object
XBee xbee = XBee();

uint8_t payload[] = { 0, 0 };

// SH + SL Address of receiving XBee
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40E3CD1E);
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));

void setup() 
{
    Serial.begin(9600);
    xbee.setSerial(Serial);
}

void loop() 
{
    // break down 10-bit reading into two bytes and place in payload
    int value = 0;
    payload[0] = value >> 8 & 0xff;
    payload[1] = value & 0xff;
    xbee.send(zbTx);
    
    delay(500);

    value = 1;
    payload[0] = value >> 8 & 0xff;
    payload[1] = value & 0xff;
    xbee.send(zbTx);
    
    delay(500);
}
