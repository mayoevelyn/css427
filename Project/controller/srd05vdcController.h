#ifndef SRD05VDC_CONTROLLER_H
#define SRD05VDC_CONTROLLER_H

#include <arduino.h>

class SRD05vdcController
{
public:
    SRD05vdcController(byte dataPin);
    ~SRD05vdcController();

    void setOn();
    void setOff();
    void toggle();
    bool isOn();
    
private:
    byte dataPin;
    bool on;
};

#endif

