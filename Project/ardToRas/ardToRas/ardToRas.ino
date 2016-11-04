#include <SoftwareSerial.h>
#define RXPIN 0
#define TXPIN 1
#define BAUDRATE 9600

SoftwareSerial XBeeSerial =  SoftwareSerial(RXPIN, TXPIN);
 
void setup()
{
    pinMode(13, OUTPUT);
    // bauderate + print
    Serial.begin(BAUDRATE);
    Serial.println("[INFOS] Arduino 1 ok.");
 
    // Envois un message aux modules xbee
    XBeeSerial.begin(BAUDRATE);
    XBeeSerial.println("[INFOS] Arduino 1 ok.");
}
 
void loop()
{

    int chaleurIntPin = 10;
    char c[256];
    char dfcon;
    
    //int inTmp36 = analogRead(chaleurIntPin);
    int inTmp36 = 80;
    
    //Serial.println(inTmp36);
    //float voltage = inTmp36 * 0.5;
    //voltage /= 1024.0;
    
    //Serial.println(voltage);
 
    // 5/1023
    float temperatureC = ((inTmp36*0.004882) - 0.50) * 100 ;
    
    Serial.print("Temp : ");Serial.print(temperatureC);
    //dfcon = Serial.print(temperatureC);
    
    
    
    // Affiche les informations qu'il reçois
    if (XBeeSerial.available()) {
        Serial.println(XBeeSerial.read());
    }
 
    //if (Serial.available() > 0) {
     // XBeeSerial.print(dfcon);
    //}
 
    delay(2000);
}
