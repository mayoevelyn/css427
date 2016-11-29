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

