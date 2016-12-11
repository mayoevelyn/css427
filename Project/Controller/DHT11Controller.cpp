// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - DHT11 Temperature and Humidity Sensor Wrapper
// ----------------------------------------------------------------------------

#include "DHT11Controller.h"

// Constructor
DHT11Controller::DHT11Controller(byte dataPin)
{
    this->dataPin = dataPin;
}

// Destructor
DHT11Controller::~DHT11Controller()
{
    // nothing to destruct
}

// Get Temp Reading
int DHT11Controller::getTempReading()
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
int DHT11Controller::getHumidityReading()
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

