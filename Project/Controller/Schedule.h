#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <arduino.h>

class Schedule
{
public:
    Schedule();
    ~Schedule();

    void setSchedule(byte zone, byte hour, byte minute, byte duration);
    byte getDuration(byte zone);
    bool checkSchedule(byte zone, byte hour, byte minute);
    
private:
    const static byte TOTAL_ZONES = 1;
    
    typedef struct
    {
        byte zone;
        byte hour;
        byte minute;
        byte duration;
        bool enabled;  
    } Zone;

    Zone collection[TOTAL_ZONES];
};

#endif
