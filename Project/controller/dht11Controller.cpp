#include "dht11Controller.h"

// Constructor
dht11Controller::dht11Controller(byte dataPin)
{
    this->dataPin = dataPin;
}

// Destructor
dht11Controller::~dht11Controller()
{
    // nothing to destruct
}

// Get Temp Reading
int dht11Controller::getTempReading()
{
    DHT_Unified dht(dataPin, DHTTYPE);
    dht.begin();
    
    // Get temperature event and print its value.
    sensors_event_t event;  
    dht.temperature().getEvent(&event);

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
    DHT_Unified dht(dataPin, DHTTYPE);
    dht.begin();
    
    // Get humidity event and print its value.
    sensors_event_t event; 
    dht.humidity().getEvent(&event);
    
    if (isnan(event.relative_humidity))
    {
        // Error reading humidity
        return -1;
    }
    
    return event.relative_humidity;
}

