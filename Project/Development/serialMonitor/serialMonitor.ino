#include <SoftwareSerial.h>

// Setup device interconnect over serial
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
    Serial.begin(9600);
    
    delay(5000);
    Serial.println("Booting Arduino Micro");

    // Set the data rate for the SoftwareSerial port
    mySerial.begin(4800);
}

void loop() {
    if (mySerial.available())
    {
        Serial.write(mySerial.read());
        delay(1);
    }
}
