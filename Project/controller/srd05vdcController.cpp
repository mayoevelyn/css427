#include "srd05vdcController.h"

// Constructor
srd05vdcController::srd05vdcController(byte dataPin)
{
    this->dataPin = dataPin;
    
    pinMode(dataPin, OUTPUT);
    digitalWrite(dataPin, LOW);
    
    on = false;
}

// Destructor
srd05vdcController::~srd05vdcController()
{
    // nothing to destruct
}

// Set On
void srd05vdcController::setOn()
{
    digitalWrite(dataPin, HIGH);
    on = true;
}

// Set Off
void srd05vdcController::setOff()
{
    digitalWrite(dataPin, LOW);
    on = false;
}

// Toggle
void srd05vdcController::toggle()
{
    if (on)
    {
        setOff();
    }
    else
    {
        setOn();
    }
}

// Is On
bool srd05vdcController::isOn()
{
    return on;
}

