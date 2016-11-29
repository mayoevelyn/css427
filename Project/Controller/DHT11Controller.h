#ifndef DHT11_CONTROLLER_H
#define DHT11_CONTROLLER_H

#include <arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11

class DHT11Controller
{
public:
    DHT11Controller(byte dataPin);
    ~DHT11Controller();

    int getTempReading();
    int getHumidityReading();
    
private:
    byte dataPin;
};

#endif
