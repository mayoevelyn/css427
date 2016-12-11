// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - Moisture Sensor Wrapper
// ----------------------------------------------------------------------------

#include "YL38Controller.h"

// Constructor
YL38Controller::YL38Controller(byte analogPin)
{
    this->analogPin = analogPin;
}

// Destructor
YL38Controller::~YL38Controller()
{
    // nothing to destruct
}

// Get Reading
int YL38Controller::getReading()
{
    return analogRead(analogPin);
}

