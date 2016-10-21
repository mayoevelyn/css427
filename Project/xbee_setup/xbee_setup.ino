#include <Printers.h>
#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial XBee(0, 1); // RX, TX

void setup() 
{
    XBee.begin(9600);
    Serial.begin(9600);
}

void loop() 
{
    if (Serial.available())
    {
        XBee.write(Serial.read());
    }
    if (XBee.available())
    {
        Serial.write(XBee.read());
    }
    
}
