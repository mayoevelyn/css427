#ifndef DHT11_CONTROLLER_H
#define DHT11_CONTROLLER_H

#include <arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11

class dht11Controller
{
public:
    dht11Controller(byte dataPin);
    ~dht11Controller();

    int getTempReading();
    int getHumidityReading();
    
private:
    byte dataPin;
};

#endif
