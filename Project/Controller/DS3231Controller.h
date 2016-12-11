// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - DS3231 Real Time Clock Wrapper
// ----------------------------------------------------------------------------

#ifndef DS3231_CONTROLLER_H
#define DS3231_CONTROLLER_H

#include <arduino.h>
#include <Wire.h>

class DS3231Controller
{
public:
    DS3231Controller();
    DS3231Controller(byte address);
    ~DS3231Controller();

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
    byte decToBcd(byte val);
    byte bcdToDec(byte val);

    // Global Variables
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

