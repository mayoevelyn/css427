#include "dht11Controller.h"

// Constructor
dht11Controller::dht11Controller(byte dataPin)
{
    *dht = DHT_Unified(dataPin, DHTTYPE);
    dht->begin();
}

// Destructor
dht11Controller::~dht11Controller()
{
    // nothing to destruct
}

// Get Temp Reading
int dht11Controller::getTempReading()
{
    // Get temperature event and print its value.
    sensors_event_t event;  
    dht->temperature().getEvent(&event);

    if (isnan(event.temperature))
    {
        // Error reading temperature
        return -1; 
    }
    
    return event.temperature;
}

// Get Humidity Reading
int dht11Controller::getHumidityReading()
{
    // Get humidity event and print its value.
    sensors_event_t event; 
    dht->humidity().getEvent(&event);
    
    if (isnan(event.relative_humidity))
    {
        // Error reading humidity
        return -1;
    }
    
    return event.relative_humidity;
}

