#ifndef RTC_CONTROLLER_H
#define RTC_CONTROLLER_H

#include "Wire.h"

class rtcController
{
public:
    rtcController(byte address);
    ~rtcController();

    void setHardwareTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
    void updateTime();
    byte getSecond();
    byte getMinute();
    byte getHour();
    byte getDayOfWeek();
    byte getDay();
    byte getMonth();
    byte getYear();
    byte* getTimeRaw();
    String getTimeString();
    
private:
    void decToBcd(byte val);
    byte bcdToDec(byte val);
    
    byte DS3231_I2C_ADDRESS;

    byte second;
    byte minute;
    byte hour;
    byte dayOfWeek;
    byte day;
    byte month;
    byte year;
};

#endif

