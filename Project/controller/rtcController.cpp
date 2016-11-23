#include "rtcController.h"

// Constructor
rtcController::rtcController(byte address)
{
    DS3231_I2C_ADDRESS = address;
    Wire.begin();
}

// Destructor
rtcController::~rtcController()
{
    // nothing to destruct
}

// Set Hardware Time
void rtcController::setHardwareTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(day)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

// Update Time
void rtcController::updateTime()
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    
    Wire.requestFrom(DS3231_I2C_ADDRESS, (byte)7);
    // request seven bytes of data from DS3231 starting from register 00h
    second = bcdToDec(Wire.read() & 0x7f);
    minute = bcdToDec(Wire.read());
    hour = bcdToDec(Wire.read() & 0x3f);
    dayOfWeek = bcdToDec(Wire.read());
    day = bcdToDec(Wire.read());
    month = bcdToDec(Wire.read());
    year = bcdToDec(Wire.read());
}

// Get Second
byte rtcController::getSecond()
{
    return second;
}

// Get Minute
byte rtcController::getMinute()
{
    return minute;
}

// Get Hour
byte rtcController::getHour()
{
    return hour;
}

// Get Day of Week
byte rtcController::getDayOfWeek()
{
    return dayOfWeek;
}

// Get Day
byte rtcController::getDay()
{
    return day;
}

// Get Month
byte rtcController::getMonth()
{
    return month;
}

// Get Year
byte rtcController::getYear()
{
    return year;
}

// Get Time Raw
byte* rtcController::getTimeRaw()
{
    byte currentTime[7];
    currentTime[0] = second;
    currentTime[1] = minute;
    currentTime[2] = hour;
    currentTime[3] = dayOfWeek;
    currentTime[4] = day;
    currentTime[5] = month;
    currentTime[6] = year;
    return currentTime;
}

// Get Time String
String rtcController::getTimeString()
{
    // Format output: HH:MM:SS, Weekday MM/DD/YY   
    String output;

    // Record hour
    output = String(hour, DEC) + ":";

    // Record minute
    if (minute < 10)
    {
        output += "0";
    }
    output += String(minute, DEC) + ":";

    // Record second
    if (second < 10)
    {
        output += "0";
    }
    output += String(second, DEC) + ", ";

    // Record weekday
    switch (dayOfWeek)
    {
        case 1:
            output += "Sunday ";
            break;
        case 2:
            output += "Monday ";
            break;
        case 3:
            output += "Tuesday ";
            break;
        case 4:
            output += "Wednesday ";
            break;
        case 5:
            output += "Thursday ";
            break;
        case 6:
            output += "Friday ";
            break;
        case 7:
            output += "Saturday ";
            break;
    }

    // Record month
    output += String(month, DEC) + "/";

    // Record day
    output += String(day, DEC) + "/";

    // Record year
    output += String(year, DEC);

    return output;
}

// Dec To BCD
// Convert normal decimal numbers to binary coded decimal
byte rtcController::decToBcd(byte val)
{
    return ((val / 10 * 16) + (val % 10));
}

// BCD to Dec
// Convert binary coded decimal to normal decimal numbers
byte rtcController::bcdToDec(byte val)
{
    return ((val / 16 * 10) + (val % 16));
}
