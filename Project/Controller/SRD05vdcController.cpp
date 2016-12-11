// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Yang Peng
//
// Controller  - SRD05VDC Relay Wrapper
// ----------------------------------------------------------------------------

#include "SRD05vdcController.h"

// Constructor
SRD05vdcController::SRD05vdcController(byte dataPin)
{
    this->dataPin = dataPin;
    
    pinMode(dataPin, OUTPUT);
    digitalWrite(dataPin, LOW);
    
    on = false;
}

// Destructor
SRD05vdcController::~SRD05vdcController()
{
    // nothing to destruct
}

// Set On
void SRD05vdcController::setOn()
{
    digitalWrite(dataPin, HIGH);
    on = true;
}

// Set Off
void SRD05vdcController::setOff()
{
    digitalWrite(dataPin, LOW);
    on = false;
}

// Toggle
void SRD05vdcController::toggle()
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
bool SRD05vdcController::isOn()
{
    return on;
}

