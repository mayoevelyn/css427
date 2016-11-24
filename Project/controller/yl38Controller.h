#ifndef YL38_CONTROLLER_H
#define YL38_CONTROLLER_H

#include <arduino.h>

class yl38Controller
{
public:
    yl38Controller(byte analogPin);
    ~yl38Controller();

    int getReading();
    
private:
    byte analogPin;
};

#endif

