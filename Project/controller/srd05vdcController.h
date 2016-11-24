#ifndef SRD05VDC_CONTROLLER_H
#define SRD05VDC_CONTROLLER_H

#include <arduino.h>

class srd05vdcController
{
public:
    srd05vdcController(byte dataPin);
    ~srd05vdcController();

    void setOn();
    void setOff();
    void toggle();
    bool isOn();
    
private:
    byte dataPin;
    bool on;
};

#endif

