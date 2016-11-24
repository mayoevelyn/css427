#include "yl38Controller.h"

// Constructor
yl38Controller::yl38Controller(byte analogPin)
{
    this->analogPin = analogPin;
}

// Destructor
yl38Controller::~yl38Controller()
{
    // nothing to destruct
}

// Get Reading
int yl38Controller::getReading()
{
    return analogRead(analogPin);
}

