// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Yang Peng
//
// Controller  - Moisture Sensor Wrapper
// ----------------------------------------------------------------------------

#ifndef YL38_CONTROLLER_H
#define YL38_CONTROLLER_H

#include <arduino.h>

class YL38Controller
{
public:
    YL38Controller(byte analogPin);
    ~YL38Controller();

    int getReading();
    
private:
    byte analogPin;
};

#endif

