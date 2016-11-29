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

